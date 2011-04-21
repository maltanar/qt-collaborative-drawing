#include "messagetransceiver.h"

// The MessageTransciever class is a generic sender / receiver for collaboration messages
// It is server/client agnostic, meaning device can run the same MessageTransciever class
// without regard to its role as client or server. It is responsible for:
// - sending collaboration messages to remote transceivers
// - receiving collaboration messages from remote transceivers

// MessageTransceiver is mainly useful for keeping a single connection to each peer
// in the network and accessing these connections as needed


MessageTransceiver::MessageTransceiver(QObject *parent) :
    QThread(parent)
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
    // start listening for connection
    if(!mServer->listen(QHostAddress::Any, TRANSCEIVER_TCP_PORT)) {
        // listening on this specified port failed
        // sorry, no go!
        qWarning() << "QTcpServer failed to listen!";
        return;
    }
    exec();
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

    // connect the signals for this tcp socket
    // TODO also connect and handle error signals
    connect(newConnection, SIGNAL(connected()), this, SLOT(connected()));
    connect(newConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    // try to establish the connection
    newConnection->connectToHost(destination, TRANSCEIVER_TCP_PORT);
}

void MessageTransceiver::sendMessage(QString destination, QByteArray msg)
{
    QTcpSocket * destinationSocket = mOpenConnections.value(destination, NULL);
    if(!destinationSocket) {
        // TODO display error
        qWarning() << "Connection for destination" << destination << "does not exist!";
        return;
    }
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
    }
}

void MessageTransceiver::disconnected()
{
    QTcpSocket * oldConnection = qobject_cast<QTcpSocket *>(sender());
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

    if(connection) {
        qWarning() << "got new data from" << origin;
        emit newData(origin, connection->readAll());
    }
}
