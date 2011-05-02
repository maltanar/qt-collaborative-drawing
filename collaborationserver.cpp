#include "collaborationserver.h"

// the CollaborationServer class acts as the backbone of the p2p/server-client
// hybrid collaboration architecture. its primary responsibilities are:
// - broadcast its own IP to the local network over UDP 45455 so that clients
//   can find it
// - keep track of active clients via login/logout messages
// - keep track of collaboration sessions and their participants, store the current
//   state of the collaborative drawing

// all actual communication is meant to be handled by the ProtocolHandler class,
// via signal/slot connections to the CollaborationServer

CollaborationServer::CollaborationServer(QObject *parent) :
    QObject(parent)
{
    m_protocolHandler = NULL;
    connect(&serviceBroadcastTimer, SIGNAL(timeout()), this, SLOT(serviceBroadcastTimeout()));
    serviceBroadcastTimer.start(SERVICE_BROADCAST_PERIOD_MS);
}

void CollaborationServer::receivedLoginRequest(QString userName)
{
    // the specified user wants to log in
    // check if there is a user with this username already logged in
    if(m_userList.contains(userName)) {
        // user with this name already logged in
        qWarning() << userName << "requested to log in but failed, already exists";
        emit sendLoginResponse(userName, 0, "User " + userName + " already exists!");
        return;
    } else {
        // add user to list of logged in users
        m_userList.append(userName);
        qWarning() << userName << "logged in successfully";
        qWarning() << "list of inloggad users" << m_userList;
        emit sendLoginResponse(userName, 1, "Login for user " + userName + " successful!");
    }
}

void CollaborationServer::receivedLogoutRequest(QString userName)
{
    // the specified user wants to log out
    // check if there is a user with this username already logged in
    if(m_userList.contains(userName)) {
        // user with this name already logged in
        // so we can log him/her out
        // TODO remove user from existing sessions and do necessary updates
        m_userList.removeOne(userName);
        qWarning() << userName << "logged out successfully";
        return;
    } else {
        // user is not in the logged in list, can't log out
        // do nothing
        qWarning() << userName << "requested to log out but was not logged in!";
        return;
    }
}

void CollaborationServer::receivedPictureRequest(QString userName, QString sessionName)
{

}

void CollaborationServer::receivedSessionJoinRequest(QString userName, QString sessionName, QString password)
{

}

void CollaborationServer::receivedSessionLeaveRequest(QString userName, QString sessionName)
{

}

void CollaborationServer::receivedSessionListRequest(QString userName)
{
    qWarning() << userName << "wants list of sessions, sending";
    emit sendSessionListResponse(userName, m_sessionList);
}

void CollaborationServer::receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData)
{

}

void CollaborationServer::receivedWritePermissionRequest(QString userName)
{

}

// sets the ProtocolHandler for this CollaborationServer
void CollaborationServer::setProtocolHandler(ProtocolHandler * newProtocolHandler)
{
    if(m_protocolHandler) {
        // disconnect all signals and slots from previous protocol handler
        disconnect(this);
        disconnect(m_protocolHandler);
    }
    // connect signals and slots for new protocol handler
    // signals from server to the protocol handler slots
    connect(this, SIGNAL(sendLoginResponse(QString,char,QString)), newProtocolHandler, SLOT(sendLoginResponse(QString,char,QString)));
    connect(this, SIGNAL(sendPictureResponse(QString,QString,QByteArray)), newProtocolHandler, SLOT(sendPictureResponse(QString,QString,QByteArray)));
    connect(this, SIGNAL(sendSessionJoinResponse(QString,QString,char,QHash<QString,long>)), newProtocolHandler, SLOT(sendSessionJoinResponse(QString,QString,char,QHash<QString,long>)));
    connect(this, SIGNAL(sendSessionLeaveResponse(QString,QString,char)), newProtocolHandler, SLOT(sendSessionLeaveResponse(QString,QString,char)));
    connect(this, SIGNAL(sendSessionListResponse(QString,QStringList)), newProtocolHandler, SLOT(sendSessionListResponse(QString,QStringList)));
    connect(this, SIGNAL(sendSessionMemberUpdate(QString,QString,char,QHash<QString,long>)), newProtocolHandler, SLOT(sendSessionMemberUpdate(QString,QString,char,QHash<QString,long>)));
    connect(this, SIGNAL(sendWritePermissionStatus(QString,QChar)),newProtocolHandler, SLOT(sendWritePermissionStatus(QString,QChar)));
    // signals from protocol handler to server slots
    connect(newProtocolHandler, SIGNAL(receivedLoginRequest(QString)), this, SLOT(receivedLoginRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedLogoutRequest(QString)), this, SLOT(receivedLogoutRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedPictureRequest(QString,QString)), this, SLOT(receivedPictureRequest(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionJoinRequest(QString,QString,QString)), this, SLOT(receivedSessionJoinRequest(QString,QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionLeaveRequest(QString,QString)), this, SLOT(receivedSessionLeaveRequest(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionListRequest(QString)), this, SLOT(receivedSessionListRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedUpdateDrawing(QString,QString,QByteArray)), this, SLOT(receivedUpdateDrawing(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedWritePermissionRequest(QString)), this, SLOT(receivedWritePermissionRequest(QString)));
    m_protocolHandler = newProtocolHandler;
}

ProtocolHandler * CollaborationServer::getProtocolHandler()
{
    return m_protocolHandler;
}

// broadcast the server's address(es) over UDP at certain intervals
// so that clients in the network can auto-discover the server
void CollaborationServer::serviceBroadcastTimeout()
{
    QByteArray broadcastPackage;
    QDataStream packageStream(&broadcastPackage, QIODevice::ReadWrite);
    packageStream << QString("WTCOLSRV");
    QNetworkInterface interface;
    QList<QHostAddress> IpList = interface.allAddresses();
    // TODO we are broadcasting only the IPv4 addresses - bad workaround
    // client side should pick the proper address in subnet
    for (int i = 0; i < IpList.size(); i++)
        if (IpList.at(i) != QHostAddress("127.0.0.1") && IpList.at(i).protocol() == QAbstractSocket::IPv4Protocol) { // local loopback isn't useful for others
            packageStream << IpList.at(i);
        }

    serviceBroadcastSocket.writeDatagram(broadcastPackage, QHostAddress::Broadcast, SERVICE_BROADCAST_PORT);
}
