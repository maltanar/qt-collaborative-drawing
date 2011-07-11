#include "messagetransceiver.h"

// The MessageTransciever class is a generic sender / receiver for collaboration messages
// It is server/client agnostic, meaning device can run the same MessageTransciever class
// without regard to its role as client or server. It is responsible for:
// - sending collaboration messages to remote transceivers
// - receiving collaboration messages from remote transceivers

// MessageTransceiver is mainly useful for keeping a single connection to each peer
// in the network and accessing these connections as needed


MessageTransceiver::MessageTransceiver(QObject *parent) :
    QObject(parent)
{
    // the transciever will act both as a server and a client
    // client behaviour is triggered by the connectTo slot
    // server behaviour is provided by a QTcpServer object
}

void MessageTransceiver::run()
{
    // set up the tcp server
    mServer = new QTcpServer();
    // forward incoming connection requests from the tcp server to us
    connect(mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    //Start the timer for sending Keep Alive message to the peers every second
    QTimer *timer = new QTimer(this);
    qWarning() << "TIMER HAS BEEN INITIATED";
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    //Set the frequency of the message to 1000 msecs
    timer->start(KEEP_ALIVE_PERIOD);

    // start listening for connection
    if(!mServer->listen(QHostAddress::Any, TRANSCEIVER_TCP_PORT)) {
        // listening on this specified port failed
        // sorry, no go!
        qWarning() << "QTcpServer failed to listen!";
        return;
    }
    //exec();
}

void MessageTransceiver::connectTo(QString destination)
{
    if(mOpenConnections.value(destination, NULL)) {
        // a connection for this destination already exists
        // TODO display error? or just return from the function quietly?
        qWarning() << "Connection for destination" << destination << "already exists!";
        return;
    }

    // TODO connect signals from QTcpSockets to handle errors and check if conn is established
    QTcpSocket * newConnection = new QTcpSocket(this);

    // we may have to queue some messages to this socket before connection
    // is established
    if(!destBuffers.contains(destination))
        destBuffers[destination] = QByteArray();

    // connect the signals for this tcp socket
    connect(newConnection, SIGNAL(connected()), this, SLOT(connected()));
    connect(newConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(newConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    // try to establish the connection
    newConnection->connectToHost(destination, TRANSCEIVER_TCP_PORT);
}

void MessageTransceiver::sendMessage(QString destination, QByteArray msg)
{
    qint32 messageSize = msg.length() + 12;   // TODO header size should be defined
    QTcpSocket * destinationSocket = mOpenConnections.value(destination, NULL);

    if(!destinationSocket) {
        qWarning() << "Connection for destination" << destination << "does not exist, put data into queue";
        // queue data, will be sent when connection is established
        if(!destBuffers.contains(destination))
            destBuffers[destination] = QByteArray();
        destBuffers[destination].append(TRANSCEIVER_HEADER);
        destBuffers[destination].append((const char *) &messageSize, 4);
        destBuffers[destination].append(msg);
        // open a connection to this peer
        connectTo(destination);
        return;
    }
    // attach the transceiver-level header with message size info
    destinationSocket->write(TRANSCEIVER_HEADER);
    destinationSocket->write((const char *) &messageSize, 4);
    // TODO add checksum?
    destinationSocket->write(msg);
}

void MessageTransceiver::sendMessageNoHeader(QTcpSocket* destinationSocket,QByteArray msg)
{
    // send message directly without attaching transceiver-level header
    // meaning that the transceiver-level header is already attached
    // only used internally for queued data transmission
    destinationSocket->write(msg);
}

void MessageTransceiver::newConnection()
{
    QTcpSocket * newSocket;
    qWarning() << "got new connection request!";
    // establish all waiting connections
    while(mServer->hasPendingConnections()) {
        newSocket = mServer->nextPendingConnection();
        qWarning() << "adding new remote-initiated connection to" << newSocket->peerAddress().toString();
        mOpenConnections.insert(newSocket->peerAddress().toString(), newSocket);
        originBuffers.insert(newSocket->peerAddress().toString(), QByteArray());
        originExpectedDataSize.insert(newSocket->peerAddress().toString(), 0);
        //insert new client's ip address to the list of clients to be kept alived
        mAliveConnections.insert(newSocket->peerAddress().toString(), false);
        mTimeouts.insert(newSocket->peerAddress().toString(), 0);
        // connect signals for newly arrived connection
        connect(newSocket, SIGNAL(readyRead()), this, SLOT(dataArrived()));
        connect(newSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        // TODO also connect and handle error signals
    }
}

void MessageTransceiver::connected()
{
    QTcpSocket * newConnection = qobject_cast<QTcpSocket *>(sender());
    QString destination = newConnection->peerAddress().toString();

    if(newConnection) {
        qWarning() << "adding new local-initiated connection to" << destination;
        // insert new tcp socket into the list of open connections
        mOpenConnections.insert(destination, newConnection);
        originBuffers.insert(destination, QByteArray());
        originExpectedDataSize.insert(destination, 0);
        //insert new client's ip address to the list of clients to be kept alived
        mAliveConnections.insert(destination, false);
        mTimeouts.insert(destination, 0);

        connect(newConnection, SIGNAL(readyRead()), this, SLOT(dataArrived()));

        // if there is data waiting to be sent in the buffer to this destination,
        // now we can send it
        if(destBuffers[destination].size() > 0) {
            qWarning() << "sending queued data to" << destination << "datasize" << destBuffers[destination].size();
            sendMessageNoHeader(newConnection, destBuffers[destination]);
            // clear the send queue
            destBuffers[destination] = QByteArray();
        }

        //Start the timer for sending Keep Alive message to the peers every second
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
        timer->start(KEEP_ALIVE_PERIOD);
    }
}

void MessageTransceiver::sendKeepAlive()
{
    //Construct a keep alive message and send it to each peer
    //TODO write a message class for this
    QMap<QString, QTcpSocket *>::iterator iter;
    for (iter = mOpenConnections.begin(); iter != mOpenConnections.end(); iter++)
    {
        sendMessage(iter.key(), QString("WTC1KEEPALVE").toAscii());
        //qWarning() << "Keep alive has been sent to : " << iter.key();
    }

    //Clear the keepalives in order to be able to send it again
    clearKeepAlives();

}

void MessageTransceiver::clearKeepAlives()
{
    //Before clearing, check if any of the keep alive acknowledgments are missing
    QMap<QString, bool>::iterator iter;
    bool isFirst = false;

    for (iter = mAliveConnections.begin(); iter != mAliveConnections.end(); iter++)
    {
        //This is the case when the first element of the mAliveConnections
        //- has been erased. So we should move the iterator back to the beginning
        //- of the list.
        if (isFirst)
        {
            iter = mAliveConnections.begin();
            isFirst = false;
            //If it is already at the end:
            if (iter == mAliveConnections.end()) break;
        }


        //If the keep alive is not acknowledged, make it known that it hasn't been
        // - acknowledged
        if (!(iter.value()))
        {
            //Increase the amount of time in seconds that the peer hasn't been acknowledging!
            mTimeouts[iter.key()]++;
            //qWarning() << "For " << mTimeouts[iter.key()] << "seconds, no answer from" << iter.key() <<" !";

            //TODO Decide how many seconds should be a timeout!
            if (mTimeouts[iter.key()] >= TIMEOUT)
            {
                //Emit signal that a particular client has been disconnected
                emit clientDisconnected(iter.key());
                //Remove this client from all the lists

                mTimeouts.remove(iter.key());
                mOpenConnections.remove(iter.key());
                originBuffers.remove(iter.key());
                originExpectedDataSize.remove(iter.key());
                destBuffers.remove(iter.key());
                qWarning() << "No answer from the peer with ip address : " << iter.key();

                //Check if the element is in the beginning of the list
                //- then we should not move the iterator back as it is out
                //- of bounds.

                iter = mAliveConnections.erase(iter);

                if (iter == mAliveConnections.begin())
                {
                    if (iter == mAliveConnections.end())
                        break;
                    isFirst = true;
                    continue;
                }

                iter--;
                if (iter == mAliveConnections.end())
                    break;
            }
        }
        else
        {
            //Clear by refreshing the acknowledgements and timeouts
            mAliveConnections[iter.key()] = false;
            mTimeouts[iter.key()] = 0;
        }
    }
}


void MessageTransceiver::disconnected()
{
    QTcpSocket * oldConnection = qobject_cast<QTcpSocket *>(sender());
    // TODO destination empty when remote peer disconnects?
    QString destination = oldConnection->peerAddress().toString();

    // TODO emit a signal for disconnection of this client

    if(oldConnection) {
        qWarning() << "peer disconnected:" << destination;
        // remove from connection list
        mOpenConnections.remove(destination);
    }
}


void MessageTransceiver::dataArrived()
{
    QTcpSocket * connection = qobject_cast<QTcpSocket *>(sender());
    QString origin = connection->peerAddress().toString();
    QByteArray newData;
    QByteArray bufferContent;
    qint32 expectedLength = 0;

    if(connection) {
        //qWarning() << "got new data from" << origin;
        newData = connection->readAll();
        // TODO check if origin buffer for this origin exists
        if(!originBuffers.contains(origin))
            originBuffers[origin] = QByteArray();
        // append data to buffer and process
        originBuffers[origin].append(newData);
        processOriginBuffer(origin);
    }
}

void MessageTransceiver::processOriginBuffer(QString origin)
{
    QByteArray originBuffer = originBuffers[origin];
    QByteArray currentMessage;
    qint32 currentMessageSize = 0;
    bool remainingMessages = true;

    while(remainingMessages) {
        if(originBuffer.length() == 0)
            break;
        // check if buffer starts with broken message
        if(!originBuffer.startsWith(TRANSCEIVER_HEADER)) {
            // buffer does not stat with header,
            // possibly broken message
            qWarning() << "Error! processOriginBuffer found missing transceiver header for origin" << origin;
            // reset the origin buffer
            // TODO instead of reset, search for next message transceiver header
            originBuffers[origin] = QByteArray();
            return;
        }
        // read the size of next message in line
        memcpy(&currentMessageSize, originBuffer.constData() + 8, 4);
        qWarning() << "processOriginBuffer current message size" << currentMessageSize << "buffer length" << originBuffer.length();
        // check if this message is complete
        if(currentMessageSize <= originBuffer.length()) {
            // message is complete
            currentMessage = originBuffer.left(currentMessageSize).right(currentMessageSize-12); // discard msgtxrx header
            qWarning() << "length of current message" << currentMessage.length();

            //Check if the message is of type keep alive, and if so, do not need to
            // inform upper layers for this.
            //TODO Make it reasonable by not giving static string here and
            // - maybe a class for keep alive message would be better
            if (currentMessage.startsWith("WTC1KEEPALVE"))
            {
                mAliveConnections[origin] = true;
            }
            else
            {
                emit gotNewData(origin, currentMessage);
            }
            // truncate the origin buffer
            originBuffers[origin] = originBuffer.right(originBuffer.length() - currentMessageSize);
            originBuffer = originBuffers[origin];
            qWarning() << "new origin buffer size" << originBuffer.size();
        } else {
            // message is incomplete, exit loop
            qWarning() << "processOriginBuffer waiting for more data";
            remainingMessages = false;
        }
    }
}

void MessageTransceiver::socketError(QAbstractSocket::SocketError err)
{
    QTcpSocket * connection = qobject_cast<QTcpSocket *>(sender());
    QString origin = connection->peerAddress().toString();

    qWarning() << "socket error" << err << "for" << origin;
}
