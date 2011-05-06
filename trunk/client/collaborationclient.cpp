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
    connect(newProtocolHandler, SIGNAL(receivedSessionMemberUpdate(QString,QString,char,QString)), this, SLOT(receivedSessionMemberUpdate(QString,QString,char,QString)));
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
        qWarning() << "Login is unsuccessful";
        emit loginResult(false, infoMsg);
    }
    else
    {
        qWarning() << "Login is successful";
        emit loginResult(true, infoMsg);
        emit sendSessionListRequest(COLLABORATION_SERVER_NAME);
    }
}

void CollaborationClient::receivedPeerHandshake(QString userName, QString sessionName)
{
    //Connect to the user that has just said hello.

    //If this client is the one which joins to a session
    // - it is expecting to get peerHandshake messages from all the session members
    if (m_currentState[sessionName] == JOIN_SESSION_STATE)
    {
        qWarning() << "Peer " << userName << " is acknowledged";
        m_collaborationSessions[sessionName]->acknowledgePeer(userName);
        //if all members shaked hands, then request picture from server
        if (m_collaborationSessions[sessionName]->isAllAcknowledged())
        {
            qWarning() << "All peers are acknowledged.";
            emit sendPictureRequest(COLLABORATION_SERVER_NAME,sessionName);
        }
        m_currentState[sessionName] = JOIN_SESSION_PEERHANDHAKE_COMPLETED;
    }
    else if (m_currentState[sessionName] == MEMBER_UPDATE_JOIN_BEGIN_RECEIVED)
    {
        //Add user to the session member list for this client
        m_collaborationSessions[sessionName]->addSessionParticipant(userName, m_collaborationSessions[sessionName]->getSessionPassword(), 0);
        //Shake hands with the client
        emit sendPeerHandshake(userName, sessionName);
    }
    else
    {
        qWarning() << "Received unexpected peer handshake";
    }
}

void CollaborationClient::receivedPictureResponse(QString userName, QString sessionName, QByteArray picData)
{
    //Show the pictureData on screen
    QPicture sessState;
    sessState.setData(picData.constData(), picData.size());
    //Set the current picture data of the session for this client
    m_collaborationSessions[sessionName]->addDrawingStep(sessState);
    //Joining to the session has been completed.
    m_currentState[sessionName] = JOIN_SESSION_COMPLETED;
}

void CollaborationClient::receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users)
{
    if (result == 0)
    {
        //Session join request was unsuccessful
        //TODO show error message.
        qWarning() << "Session Join by " << userName << " was unsuccessful.";
    }
    else
    {
        //Session join was successful.
        qWarning() << "Session Join by " << userName << " was successful";
        m_currentState.insert(sessionName, JOIN_SESSION_STATE);

        //Create a new session
        CollaborationSession *collaborationSession = new CollaborationSession;
        collaborationSession->setSessionName(sessionName);

        collaborationSession->setSessionParticipants(users);

        //Map it with its sessionName
        m_collaborationSessions.insert(sessionName, collaborationSession);

        //Add all members to the list that is going to which
        // - this client will establish TCP connections
        QHash<QString, long>::iterator itr;
        for (itr = users.begin(); itr != users.end(); itr++)
        {
            //Send handshake messages to the users in the session
            emit sendPeerHandshake(itr.key(), sessionName);
        }
    }
}

void CollaborationClient::receivedSessionLeaveResponse(QString userName, QString sessionName, char result)
{
    if (result == 0)
    {
        qWarning() << "Session Leave by " << userName << " was unsuccesful";
    }
    else
    {
        qWarning() << "Session Leave by " << userName << " was succesful";
        //Remove the participants from the client's collaboration session
        m_collaborationSessions[sessionName]->getSessionParticipants().clear();
        //Remove the name of the session as the client has left it
        m_collaborationSessions[sessionName]->setSessionName("");
        //Remove the password of the session as the client has left the session
        m_collaborationSessions[sessionName]->setSessionPassword("");
    }
}

void CollaborationClient::receivedSessionListResponse(QString userName, QStringList sessionList)
{
    //TODO List the sessions so that a client can decide which one to join
    int size = sessionList.size();
    qWarning() << "Current sessions:\n";

    //Check if the client has already a session list
    // - if yes, clear it and get the new list
    if (!m_sessionList.empty())
         m_sessionList.clear();

    m_sessionList.append(sessionList);

    //Print the session list to stdout
    for (int i = 0; i < size; i++)
    {
       qWarning() << i << sessionList.at(i) << "\n";   
    }

    emit sessionListAvailable(sessionList);
}

void CollaborationClient::receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QString user)
{

    //The users in the list "users" have started to join to the session
    if (updateType == UPDATE_SESSION_JOIN_BEGIN)
    {
        m_currentState[sessionName] = MEMBER_UPDATE_JOIN_BEGIN_RECEIVED;
        //TODO - This client first should complete what it is sending and stop sending
        //TODO - Open connections to the members in the list "users"
    }
    //The users in the list "users" have completely joined the session
    else if (updateType == UPDATE_SESSION_JOIN_END)
    {
        m_currentState[sessionName] = MEMBER_UPDATE_JOIN_END_RECEIVED;
        //TODO - This client resumes sending
    }
    //The users in the list "users" have left the session
    else if (updateType == UPDATE_SESSION_LEAVE)
    {
        m_collaborationSessions[sessionName]->getSessionParticipants().remove(user);
        //TODO - Close connections to the users in the list "users"
    }
}

void CollaborationClient::receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData)
{
    //Update the state with the picData
    QPicture drawingStep;
    drawingStep.setData(picData.constData(), picData.size());
    m_collaborationSessions[sessionName]->addDrawingStep(drawingStep);
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
    //qWarning() << "got service broadcast!";

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
                if (allAddresses[i].ip().protocol() != QAbstractSocket::IPv4Protocol)
                    continue;

                if (((allAddresses[i].ip().toIPv4Address() & allAddresses[i].netmask().toIPv4Address()) ==
                     (serverAddress.toIPv4Address() & allAddresses[i].netmask().toIPv4Address())))
                {
                    //qWarning() << allAddresses[i].ip() << allAddresses[i].netmask();
                    //qWarning() << serverAddress << "!";
                    emit foundCollaborationServer(serverAddress);
                    break;
                }
            }
        }
    }
}


void CollaborationClient::loginToServer(QHostAddress serverAddress, QString userName)
{
    // TODO check if mapping for server already exists
    m_protocolHandler->addUserMapping(COLLABORATION_SERVER_NAME, serverAddress.toString());
    // set the origin (source) user name
    m_protocolHandler->setUserName(userName);
    // emit login request
    emit sendLoginRequest(COLLABORATION_SERVER_NAME);
}

void CollaborationClient::refreshSessionList()
{
    emit sendSessionListRequest(COLLABORATION_SERVER_NAME);
}
