#include "messagedispatcher.h"
#include <QStringList>

MessageDispatcher::MessageDispatcher(QObject *parent) :
    QObject(parent)
{
    m_messageTransceiver = NULL;
    m_userManager = new UserManager();

    connect(m_userManager, SIGNAL(peerConnected(QString)), this, SLOT(connectionEstablished(QString,QString)));

    m_userManager->setMessageTransceiver(m_messageTransceiver);
    // create a default username
    m_userManager->setUserName(QString::number((QDateTime::currentMSecsSinceEpoch())));
}

void MessageDispatcher::subscribe(ProtocolHandler *protocolHandler, QStringList prefixes)
{
    //Each prefix is a key to a protocol handler
    //Instead of using protocol handler as a key and stringlist as a value
    //- it is prefered to use prefix as a key for a better performance while searching
    int size = prefixes.size();
    for (int i = 0; i < size; i++)
    {
        m_subscriptionList.insertMulti(prefixes.at(i), protocolHandler);
    }

    // we want all ProtocolHandler instances to be connected to the message broadcast signal
    // check if we have this particular ProtocolHandler in record
    if(!m_protocolHandlers.contains(protocolHandler)) {
        // add to our list of known handlers...
        m_protocolHandlers.append(protocolHandler);
        // .. and connect the broadcast message signal to its reception socket
        connect(this, SIGNAL(broadcastMessage(QByteArray)), protocolHandler, SLOT(receiveBroadcast(QByteArray)));
    }
}

void MessageDispatcher::unsubscribe(ProtocolHandler *protocolHandler)
{
    //Remove all the prefixes that belong to the mentioned protocol handler
    QMap<QString, ProtocolHandler *>::iterator itr;

    // remove from the list of ProtocolHandler instances
    m_protocolHandlers.removeAll(protocolHandler);
    // disconnect from the ProtocolHandler broadcast system
    disconnect(this, SIGNAL(broadcastMessage(QByteArray)), protocolHandler, SLOT(receiveBroadcast(QByteArray)));

    for (itr = m_subscriptionList.begin(); itr != m_subscriptionList.end(); itr++)
    {
        if (itr.value() == protocolHandler)
        {
            //Remove the item from the hash
            itr = m_subscriptionList.erase(itr);
            //Move the iterator once backwards
            //- to compensate itr++
            itr--;
        }
    }
}

void MessageDispatcher::receiveMessage(QString origin, QByteArray msg)
{
    QString message;
    QDataStream deserializer(&msg, QIODevice::ReadOnly);
    deserializer >> message;

    //Check according to the prefix of the message
    // to which protocol handler the message belongs to
    QMap<QString, ProtocolHandler *>::iterator itr;

    //Check if this message belongs to user manager
    if (message.startsWith(USER_HANDSHAKE_SIGNATURE))
    {
        m_userManager->receiveData(origin, msg);
    }

    for (itr = m_subscriptionList.begin(); itr != m_subscriptionList.end(); itr++)
    {
        if (message.startsWith(itr.key()))
        {
            ((ProtocolHandler *)(itr.value()))->receiveData(origin, msg);
            //No break here as there might be more than one protocol handlers
            //- expecting the same prefix
        }
    }


}

void MessageDispatcher::setMessageTransceiver(MessageTransceiver *messageTransceiver)
{
    if(m_messageTransceiver) {
        // disconnect all signals and slots from previous MessageTransceiver
        disconnect(this);
        disconnect(m_messageTransceiver);
        m_messageTransceiver = messageTransceiver;
    }
    // connect signals and slots
    connect(messageTransceiver, SIGNAL(gotNewData(QString,QByteArray)), this, SLOT(receiveMessage(QString,QByteArray)));
}

MessageTransceiver * MessageDispatcher::getMessageTransceiver()
{
    return m_messageTransceiver;
}

void MessageDispatcher::sendMessage(QString destination, QByteArray msg)
{
    if(!m_userManager) {
        qWarning() << "no UserManager set for MessageDispatcher, aborting...";
        return;
    }

    // we have a specially defined destination to broadcast to all fellow ProtocolHandler's
    if(destination == PROTOCOLHANDLER_BROADCAST) {
        // the signal is connected to ProtocolHandler::receiveBroadcast for all instances
        emit broadcastMessage(msg);
        // no need to forward this to MessageTransceiver, we can stop here
        return;
    }

    // regular username destination, forward to MessageTransceiver
    QString destinationAddress;
    destinationAddress = m_userManager->peerAddress(destination);
    // check if the destination exists as a connected peer in UserManager
    if(destinationAddress == "") {
        // no connection for this peer yet exists, request a connection
         m_userManager->requestConnection(destination);
        // buffer the message, we'll deliver it when a connection is established
        m_bufferedMessages.insertMulti(destination, msg);
    } else {
        // already an open connection, pass on to MessageTransceiver
        m_messageTransceiver->sendMessage(destinationAddress, msg);
    }

}

void MessageDispatcher::connectionEstablished(QString destination, QString destinationAddress)
{
    // we have a newly established connection to destination at destinationAddress
    // check if we have any pending data towards this destination
    while(m_bufferedMessages.count(destination)) {
        // deliver the pending messages via MessageTransceiver
        m_messageTransceiver->sendMessage(destinationAddress, m_bufferedMessages.take(destination));
    }
}

void MessageDispatcher::setUserManager(UserManager *newUserManager)
{
    if (m_userManager)
    {
        m_userManager->disconnect();
        disconnect(m_userManager);
    }
    m_userManager = newUserManager;
    connect(m_userManager, SIGNAL(peerConnected(QString)), this, SLOT(connectionEstablished(QString,QString)));
}

UserManager* MessageDispatcher::getUserManager()
{
    return m_userManager;
}
