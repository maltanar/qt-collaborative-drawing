#include "messagedispatcher.h"
#include <QStringList>

MessageDispatcher::MessageDispatcher(QObject *parent) :
    QObject(parent)
{
    m_userManager = NULL;
    m_messageTransceiver = NULL;
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
        connect(this, SIGNAL(messageBroadcastSignal(QString,QByteArray)), protocolHandler, SLOT(receiveData(QString,QByteArray)));
    }
}

void MessageDispatcher::broadcastRequestSlot(QString origin, QByteArray msg)
{
    Q_UNUSED(origin)
    // emit the broadcast message signal, which should be received by all ProtocolHandler instances
    emit signalMessageBroadcast(PROTOCOLHANDLER_BROADCAST, msg);
}

void MessageDispatcher::unsubscribe(ProtocolHandler *protocolHandler)
{
    //Remove all the prefixes that belong to the mentioned protocol handler
    QMap<QString, ProtocolHandler *>::iterator itr;
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
    // TODO Yaman: uncomment the lines about UserManager when it is implemented
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
    m_userManager = newUserManager;

    connect(m_userManager, SIGNAL(peerConnected(QString)), this, SLOT(connectionEstablished(QString,QString)));
}

UserManager* MessageDispatcher::getUserManager()
{
    return m_userManager;
}
