#include "messagedispatcher.h"
#include <QStringList>

MessageDispatcher::MessageDispatcher(QObject *parent) :
    QObject(parent)
{
}

void MessageDispatcher::subscribe(ProtocolHandler *protocolHandler, QStringList prefixes)
{
    //Each prefix is a key to a protocol handler
    //Instead of using protocol handler as a key and stringlist as a value
    //- it is prefered to use prefix as a key for a better performance while searching
    int size = prefixes.size();
    for (int i = 0; i < size; i++)
    {
        m_protocolHandlers.insert(prefixes.at(i), protocolHandler);
    }
}

void MessageDispatcher::unsubscribe(ProtocolHandler *protocolHandler)
{
    //Remove all the prefixes that belong to the mentioned protocol handler
    //TODO Might be a logical mistake with comparison of pointers
    QHash<QString, ProtocolHandler *>::iterator itr;
    for (itr = m_protocolHandlers.begin(); itr != m_protocolHandlers.end(); itr++)
    {
        if (itr.value() == protocolHandler)
        {
            //Remove the item from the hash
            itr = m_protocolHandlers.erase(itr);
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
    QHash<QString, ProtocolHandler *>::iterator itr;

    for (itr = m_protocolHandlers.begin(); itr != m_protocolHandlers.end(); itr++)
    {
        if (message.startsWith(itr.key()))
        {
            ((ProtocolHandler *)(itr.value()))->receiveData(origin, msg);
            break;
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
