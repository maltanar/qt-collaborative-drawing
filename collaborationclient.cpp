#include "collaborationclient.h"

CollaborationClient::CollaborationClient(QObject *parent) :
    QObject(parent),
    m_protocolHandler(NULL)
{
    // set up the broadcast listener to autodiscover the server
    serviceBroadcastReceiver.bind(SERVICE_BROADCAST_PORT, QUdpSocket::ShareAddress);
    connect(&serviceBroadcastReceiver, SIGNAL(readyRead()), this, SLOT(gotServiceBroadcast()));
}

void CollaborationClient::setProtocolHandler(ProtocolHandler * newProtocolHandler)
{
    if (m_protocolHandler)
    {
        disconnect(this);
        disconnect(m_protocolHandler);
    }

    connect(this, SIGNAL(sendLoginRequest(QString)), newProtocolHandler, SLOT(sendLoginRequest(QString)));
    connect(this, SIGNAL(sendLogoutRequest(QString)), newProtocolHandler, SLOT(sendLogoutRequest(QString)));
    connect(this, SIGNAL(sendPeerHandshake(QString,QString)), newProtocolHandler, SLOT(sendPeerHandshake(QString,QString)));
    connect(this, SIGNAL(sendPictureRequest(QString,QString)), newProtocolHandler, SLOT(sendPictureRequest(QString,QString)));
    connect(this, SIGNAL(sendSessionJoinRequest(QString,QString,QString)), newProtocolHandler, SLOT(sendSessionJoinRequest(QString,QString,QString)));
    connect(this, SIGNAL(sendSessionLeaveRequest(QString,QString)), newProtocolHandler, SLOT(sendSessionLeaveRequest(QString,QString)));
    connect(this, SIGNAL(sendSessionListRequest(QString)), newProtocolHandler, SLOT(sendSessionListRequest(QString)));
    connect(this, SIGNAL(sendWritePermissionRequest(QString)), newProtocolHandler, SLOT(sendWritePermissionRequest(QString)));

    connect(newProtocolHandler, SIGNAL(receivedLoginResponse(QString,QChar,QString)), this, SLOT(receivedLoginResponse(QString,QChar,QString)));
    connect(newProtocolHandler, SIGNAL(receivedPeerHandshake(QString,QString)), this, SLOT(receivedPeerHandshake(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedPictureResponse(QString,QString,QByteArray)), this, SLOT(receivedPictureResponse(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedSessionJoinResponse(QString,QString,char,uint,QHash<QString,long>)), this, SLOT(receivedSessionJoinResponse(QString,QString,char,uint,QHash<QString,long>)));
    connect(newProtocolHandler, SIGNAL(receivedSessionLeaveResponse(QString,QString,char)), this, SLOT(receivedSessionLeaveResponse(QString,QString,char)));
    connect(newProtocolHandler, SIGNAL(receivedSessionListResponse(QString,QStringList)), this, SLOT(receivedSessionListResponse(QString,QStringList)));
    connect(newProtocolHandler, SIGNAL(receivedSessionMemberUpdate(QString,QString,char,QHash<QString,long>)), this, SLOT(receivedSessionMemberUpdate(QString,QString,char,QHash<QString,long>)));
    connect(newProtocolHandler, SIGNAL(receivedUpdateDrawing(QString,QString,QByteArray)), this, SLOT(receivedUpdateDrawing(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedWritePermissionStatus(QString,QChar)), this, SLOT(receivedWritePermissionStatus(QString,QChar)));
    m_protocolHandler = newProtocolHandler;
}

ProtocolHandler * CollaborationClient::getProtocolHandler()
{
    return m_protocolHandler;
}


void CollaborationClient::receivedLoginResponse(QString userName, QChar result, QString infoMsg)
{
    //TODO show the infoMsg
    qWarning() << "Login Response message: " <<  infoMsg;
    if (result == 0)
    {
        //TODO show error message
        qWarning() << "Login is unsucessful";
    }
    else
    {
        qWarning() << "Login is sucessful";
        emit sendSessionListRequest(COLLABORATION_SERVER_NAME);
    }
}

void CollaborationClient::receivedPeerHandshake(QString userName, QString sessionName)
{
    //TODO Add user into the list, to which the drawings are sent
}

void CollaborationClient::receivedPictureResponse(QString userName, QString sessionName, QByteArray picData)
{
    //TODO Show the pictureData on screen
}

void CollaborationClient::receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users)
{

    if (result == 0)
    {
        //Session join request was unsucessful
        //TODO show error message.
    }
    else
    {
        //Session join was sucessful.
        //Add all members to the list that is going to be
        // - used to send picture data to all members in that session
        QHash<QString, long>::iterator itr;
        for (itr = users.begin(); itr != users.end(); itr++)
        {
            m_userList.append(itr.key());
        }


    }
    //TODO check the result:
    //TODO - if the sessionjoin was sucessful, get the members
    //TODO - - this client waits until all connections to the peers are established
    //TODO - - this client sends picRequest to the server
    //TODO - otherwise show message "join was unsucessful"
}

void CollaborationClient::receivedSessionLeaveResponse(QString userName, QString sessionName, char result)
{
    //TODO depending on the result, delete the members from the list that drawing is sent
    //TODO Close connections to each peer in the session so that no data comes from those peers.
}

void CollaborationClient::receivedSessionListResponse(QString userName, QStringList sessionList)
{
    //TODO List the sessions so that a client can decide which one to join
    int size = sessionList.size();
    qWarning() << "Current sessions:\n";
    for (int i = 0; i < size; i++)
    {
       qWarning() << i << sessionList.at(i) << "\n";
    }
}

void CollaborationClient::receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QHash<QString, long> users)
{
    //TODO if updateType is 0
    //TODO - This client first should complete what it is sending and stop sending
    //TODO - Open connections to the members in the list "users"
    //TODO if updateType is 1
    //TODO - This client resumes sending
}

void CollaborationClient::receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData)
{
    //TODO update the state with the picData
}

void CollaborationClient::receivedWritePermissionStatus(QString userName, QChar status)
{
    //TODO if status 0
    //TODO - cannot draw to classroom session
    //TODO else
    //TODO - can draw to classroom session
    //TODO so, change a state variable to show the permission accordingly
}

void CollaborationClient::gotServiceBroadcast()
{
    qWarning() << "got service broadcast!";

    QByteArray broadcastPackage;
    QString packageHeader;
    QHostAddress serverAddress;
    QNetworkInterface networkInterface;
    QList<QNetworkInterface> allInterfaces = networkInterface.allInterfaces();
    QList<QNetworkAddressEntry> allAddresses;

    for (int i = 0; i < allInterfaces.size(); i++)
    {
        allAddresses.append(allInterfaces[i].addressEntries());
    }

    while(serviceBroadcastReceiver.hasPendingDatagrams())
    {
        broadcastPackage.resize(serviceBroadcastReceiver.pendingDatagramSize());
        serviceBroadcastReceiver.readDatagram(broadcastPackage.data(), broadcastPackage.size());
        QDataStream packageStream(&broadcastPackage, QIODevice::ReadWrite);
        packageStream >> packageHeader;

        if (packageHeader == "WTCOLSRV")
        {
            packageStream >> serverAddress;

            for(int i = 0; i < allAddresses.size(); i++)
            {
                if (allAddresses[i].ip().protocol() != QAbstractSocket::IPv4Protocol) continue;

                if (((allAddresses[i].ip().toIPv4Address() & allAddresses[i].netmask().toIPv4Address()) ==
                     (serverAddress.toIPv4Address() & allAddresses[i].netmask().toIPv4Address())))
                {
                    qWarning() << allAddresses[i].ip() << allAddresses[i].netmask();
                    qWarning() << serverAddress << "!";
                    emit foundCollaborationServer(serverAddress);
                    break;
                }
            }
        }
    }
}
