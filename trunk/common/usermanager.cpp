#include "usermanager.h"

UserManager::UserManager(QObject *parent) :
    QObject(parent)
{
    m_messageTransceiver = NULL;
    // set up things for user discovery and announcement
    m_discoveryListenerSocket.bind(DISCOVERY_BROADCAST_PORT, QUdpSocket::ShareAddress);
    connect(&m_discoveryBroadcastTimer, SIGNAL(timeout()), this, SLOT(discoveryBroadcastTimeout()));
    connect(&m_discoveryListenerSocket, SIGNAL(readyRead()), this, SLOT(discoveryBroadcastReceived()));
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
        qWarning() << "Requested a connection to a non-existing user" << userName;
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
        disconnect(this, SIGNAL(sendMessage(QString,QByteArray)), m_messageTransceiver, SLOT(sendMessage(QString,QByteArray)));
        disconnect(m_messageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(peerDisconnected(QString)));
    }

    m_messageTransceiver = messageTransceiver;

    connect(this, SIGNAL(sendMessage(QString,QByteArray)), m_messageTransceiver, SLOT(sendMessage(QString,QByteArray)));
    connect(m_messageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(peerDisconnected(QString)));
}

void UserManager::discoveryBroadcastTimeout()
{
    QByteArray discoveryPacketData;
    QDataStream serializer(&discoveryPacketData, QIODevice::ReadWrite);

    // create a package to broadcast over UDP that announces the presence of this user
    // to the network

    serializer << DISCOVERY_BROADCAST_SIGNATURE << getUserName();

    m_discoveryBroadcastSocket.writeDatagram(discoveryPacketData,QHostAddress::Broadcast, DISCOVERY_BROADCAST_PORT);
}

bool UserManager::isDiscoveryBroadcastActive()
{
    return m_discoveryBroadcastTimer.isActive();
}

void UserManager::setDiscoveryBroadcastActive(bool active)
{
    if(active)
        m_discoveryBroadcastTimer.start(DISCOVERY_BROADCAST_PERIOD);
    else
        m_discoveryBroadcastTimer.stop();
}

void UserManager::discoveryBroadcastReceived()
{
    QByteArray discoveryPackage;
    QString packageHeader;
    QString peerName;
    QHostAddress peerAddress;
    QStringList peerAddressList;

    while(m_discoveryListenerSocket.hasPendingDatagrams())
    {
        discoveryPackage.resize(m_discoveryListenerSocket.pendingDatagramSize());
        m_discoveryListenerSocket.readDatagram(discoveryPackage.data(), discoveryPackage.size(), &peerAddress);
        QDataStream packageStream(&discoveryPackage, QIODevice::ReadWrite);
        packageStream >> packageHeader;

        if (packageHeader == DISCOVERY_BROADCAST_SIGNATURE)
        {
            // this is indeed a peer discovery package, retrieve peer name from the data
            packageStream >> peerName;
            // insert entry into peer address list if it does not already exist
            peerAddressList = m_availableUsers.value(peerName, QStringList());
            if(!peerAddressList.contains(peerAddress.toString()))
                peerAddressList.append(peerAddress.toString());
            qWarning() << "UserManager address list for peer" << peerName << ":" << peerAddressList;
            m_availableUsers[peerName] = peerAddressList;
        } else
            qWarning() << "UserManager received invalid discovery broadcast!";
    }
}
