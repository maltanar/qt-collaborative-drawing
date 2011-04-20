#include "messagetransceiver.h"

// The MessageTransciever class is a generic sender / receiver for collaboration messages
// It is server/client agnostic, meaning device can run the same MessageTransciever class
// without regard to its role as client or server. It is responsible for:
// - sending collaboration messages to remote transceivers
// - receiving collaboration messages from remote transceivers
// - destination resolution to abstract connection details

// TODO IMPORTANT
// doing the destination resolution on this level also requires that we somehow communicate the
// user name for a newly connected client on this protocol level. two possible solutions are:
// - discard username resolution, do it in the upper layer
// - send user identification / handshake message on this layer (HELLOPAL)

MessageTransceiver::MessageTransceiver(QObject *parent) :
    QThread(parent)
{
    // the transciever will act both as a server and a client
    // client behaviour is triggered by the connectTo slot
    // server behaviour is provided by a QTcpServer object

    // set up the tcp server
    mServer = new QTcpServer(this);
    // forward incoming connection requests from the tcp server to us
    connect(mServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void MessageTransceiver::run()
{
    mServer->listen(QHostAddress::Any, COLLABORATION_TCP_PORT);
    exec();
}

void MessageTransceiver::addDestinationEntry(QString destination, QString hostname)
{
    // check if given destination key already exists in the destination resolution system
    removeDestinationEntry(destination);
    // add new destination entry
    mDestinationEntries.insert(destination, hostname);
}

void MessageTransceiver::removeDestinationEntry(QString destination)
{
    // try to remove given destination entry
    if(mDestinationEntries.contains(destination))
        mDestinationEntries.remove(destination);
}


QString MessageTransceiver::resolveDestination(QString destination)
{
    // destination entry lookup, empty string if not found
    return mDestinationEntries.value(destination, "");
}

void MessageTransceiver::connectTo(QString destination)
{
    // lookup given destination in the entries table
    QString hostname = resolveDestination(destination);

    if(hostname == "") {
        // TODO SHOWERROR destination could not be resolved - display error message
        qWarning() << "Could not resolve hostname for destination" << destination;
        return;
    }

    if(mOpenConnections.value(destination, NULL)) {
        // a connection for this destination already exists
        // TODO display error? or just return from the function quietly?
        qWarning() << "Connection for destination" << destination << "already exists!";
        return;
    }

    // TODO connect signals from QTcpSockets to handle errors and check if conn is established
    QTcpSocket * newConnection = new QTcpSocket(this);
    newConnection->connectToHost(hostname, COLLABORATION_TCP_PORT);

    // insert new tcp socket into the list of open connections
    mOpenConnections.insert(destination, newConnection);
}

void MessageTransceiver::sendMessage(QString destination, WTMessage msg)
{

}

void MessageTransceiver::newConnection()
{
    qWarning() << "got new connection request!";

}
