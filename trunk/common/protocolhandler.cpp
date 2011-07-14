#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent):
    QObject(parent)
{
    m_messageTransceiver = NULL;
    m_messageDispatcher = NULL;
}

void ProtocolHandler::receiveData(QString origin, QByteArray data)
{
    // default implementation does nothing
}

void ProtocolHandler::setMessageDispatcher(MessageDispatcher * messageDispatcher)
{
    // This protocol handler has already been subscribed to the message dispatcher
    // - unsubscribe it!
    if (m_messageDispatcher)
    {
        m_messageDispatcher->unsubscribe(this);
    }

    m_messageDispatcher = messageDispatcher;

    connect(this, SIGNAL(sendMessage(QString, QByteArray)), m_messageDispatcher, SLOT(sendMessage(QString,QByteArray)));
    // we don't have a message reception slot connection since MessageDispatcher handles that itself
}

void ProtocolHandler::setMessageTransceiver(MessageTransceiver * newMesssageTransceiver)
{
    // TODO ProtocolHandler won't have direct access to MessageTransceiver, remove this
    if(m_messageTransceiver) {
        // disconnect all signals and slots from previous MessageTransceiver
        disconnect(this);
        disconnect(m_messageTransceiver);
    }
    // connect signals and slots

    //connect(this, SIGNAL(sendMessage(QString,QByteArray)), newMesssageTransceiver, SLOT(sendMessage(QString,QByteArray)));

    // TODO remove the line below - ProtocolHandler itself gets disconnection info from broadcast
    connect(newMesssageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
}

MessageTransceiver* ProtocolHandler::getMessageTransceiver()
{
    return m_messageTransceiver;
}

void ProtocolHandler::receiveBroadcast(QByteArray data)
{
    // receive broadcast
    qWarning() << "ProtocolHandler::receiveBroadcast with data size" << data.size();

    // read the broadcast signature
    QString signature;
    QDataStream deserializer(&data, QIODevice::ReadOnly);
    deserializer >> signature;

    qWarning() << "broadcast message sigature" << signature;

    if(signature == BROADCAST_USER_DISCONNECT_SIGNATURE) {
        // a user has disconnected, decode name and pass on to handler
        QString userName;
        deserializer >> userName;
        userDisconnected(userName);
    }

    // TODO handle other broadcast types here
}

void ProtocolHandler::userDisconnected(QString userName)
{
    // the default disconnection handler does nothing
    qWarning() << "peer with username" << userName << "disconnected";
}

QMap<QString, QStringList> ProtocolHandler::getAvailableUsers()
{
    if(m_messageDispatcher) {
        return m_messageDispatcher->getUserManager()->getAvailableUsers();
    } else
        return QMap<QString, QStringList>();
}

QMap<QString, QString> ProtocolHandler::getConnectedUsers()
{
    if(m_messageDispatcher) {
        return m_messageDispatcher->getUserManager()->getConnectedUsers();
    } else
        return QMap<QString, QString>();
}

QString ProtocolHandler::getLocalUserName()
{
    if(m_messageDispatcher)
        return m_messageDispatcher->getUserManager()->getUserName();
    else
        return "";
}
