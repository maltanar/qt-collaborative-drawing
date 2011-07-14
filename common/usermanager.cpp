#include "usermanager.h"

UserManager::UserManager(QObject *parent) :
    QObject(parent)
{
    m_messageTransceiver = NULL;
}

void UserManager::setUserName(QString userName)
{
    this->userName = userName;
}

QString UserManager::getUserName()
{
    return userName;
}

QMap<QString, QStringList> UserManager::getAvailableUsers()
{
    return m_availableUsers;
}

QMap<QString, QString> UserManager::getConnectedUsers()
{
    return m_connectedUsers;
}

QString UserManager::peerAddress(QString userName)
{
    //If the user is in the connected list, then return its ip address
    if (m_connectedUsers.contains(userName))
    {
        return m_connectedUsers.value(userName, NULL);
    }
    else
    {
        //The user is not in the list of connected users
        return "";
    }
}

void UserManager::requestConnection(QString userName)
{
    //In order to set connection, the user's ip addresses are obtained
    // - from the available users list

    //We try to connect by using each ip address until the correct one is found
    if (!m_availableUsers.contains(userName))
    {
        qWarning() << "Requested a connection to a non-existing user";
        return;
    }

    QStringList ipAddresses = m_availableUsers.value(userName);

    //TODO Implement this for multiple ip addresses (use timer!)

    //Send handshake to the user to be connected with
    //TODO Determine the content of the handshake message

    //Add the user to the pending list in order that
    // - it is known the client waiting for a reply
    m_pendingUsers.push_back(userName);

    qWarning() << "Requested a connection to" << userName;

    sendHandshake(ipAddresses[0]);

    /*int size = ipAddresses.size();
    for (int i = 0; i < size; i++)
    {

    }*/
}

void UserManager::receiveHandshake(QString srcUserName, QString srcIp)
{
    //A user sent a handshake

    //If this peer (receiver) has sent a handshake to this
    //- peer, then this is the response for it!
    if (m_pendingUsers.contains(srcUserName))
    {
        //As the handshake has arrived, add the user to the connected list
        m_connectedUsers.insert(srcUserName, srcIp);
        m_pendingUsers.removeOne(srcUserName);
        qWarning() << "Handshake has been confirmed by" << srcUserName;
        //User has successfully connected
        emit peerConnected(srcUserName, srcIp);
    }
    else
    {
        //This user received a handshake; therefore, a peer wants to set
        // a connection with it.
        // This user then should reply with a handshake
        m_connectedUsers.insert(srcUserName, srcIp);
        sendHandshake(srcIp);
    }
}

void UserManager::sendHandshake(QString destIp)
{
    //Construct the convenient handshake message
    QBuffer buffer;
    QDataStream stream;
    QByteArray data;

    buffer.setBuffer(&data);
    stream.setDevice(&buffer);
    buffer.open(QIODevice::ReadWrite);

    stream << QString(USER_HANDSHAKE_SIGNATURE) << userName;

    buffer.close();

    qWarning() << "Handshake has been sent to" << destIp;

    //Send the message to the transport layer
    emit sendMessage(destIp, data);
}


void UserManager::receiveData(QString srcIp, QByteArray data)
{
    //New data arrived, parse it
    QString srcUserName;
    QString signature;

    QBuffer buffer;
    QDataStream stream;
    buffer.setBuffer(&data);
    stream.setDevice(&buffer);
    buffer.open(QIODevice::ReadOnly);

    stream >> signature >> srcUserName;

    buffer.close();

    //The username and the ip of the sender are obtained
    receiveHandshake(srcUserName, srcIp);
}


void UserManager::peerDisconnected(QString peerUserName)
{
    //Tell messageDispatcher to broadcast all the protocolhandlers
    //- that a user with the username peerUserName has been disconnected
    m_connectedUsers.remove(peerUserName);

    QByteArray data;
    QBuffer buffer;
    QDataStream stream;
    buffer.setBuffer(&data);
    stream.setDevice(&buffer);
    buffer.open(QIODevice::ReadWrite);

    stream << QString(BROADCAST_USER_DISCONNECT_SIGNATURE) << peerUserName;

    buffer.close();

    emit sendMessage(PROTOCOLHANDLER_BROADCAST, data);
}


MessageTransceiver * UserManager::getMessageTransceiver()
{
    return m_messageTransceiver;
}

void UserManager::setMessageTransceiver(MessageTransceiver * messageTransceiver)
{
    //Disconnect all the signals if there was already a message transceiver
    if (m_messageTransceiver)
    {
        m_messageTransceiver->disconnect();
    }

    m_messageTransceiver = messageTransceiver;

    connect(this, SIGNAL(sendMessage(QString,QByteArray)), m_messageTransceiver, SLOT(sendMessage(QString,QByteArray)));
    connect(m_messageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(peerDisconnected(QString)));
}
