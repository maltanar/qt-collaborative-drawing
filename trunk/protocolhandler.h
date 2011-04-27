#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QHash>

#include "wtmessage.h"
#include <wtloginmessage.h>
#include <wtloginresponse.h>
#include <wtlogoutrequest.h>
#include <wtpicturerequest.h>
#include <wtpictureresponse.h>
#include <wtsessionjoinrequest.h>
#include <wtsessionjoinresponse.h>
#include <wtsessionleaverequest.h>
#include <wtsessionleaveresponse.h>
#include <wtsessionlistrequest.h>
#include <wtsessionlistresponse.h>
#include <wtsessionmemberupdate.h>
#include <wtupdatedrawing.h>
#include <wtwritepermissionrequest.h>
#include <wtwritepermissionstatus.h>
#include <wtpeerhandshake.h>

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);

private:
    QHash<QString, QString> peerMap;
    QHash<QString, QString> pendingMapRequests;
    QString userName;

    bool deliverMessage(WTMessage * msg);

    QString getUserName();
    void setUserName(QString username);

    void handleMapRequestStatus(QString username, bool confirmed, QString destination = "");

    void handleLoginRequest(WTLoginMessage *msg, QString requestOrigin);
    void handleLoginResponse(WTLoginResponse *msg);
    void handleLogoutRequest(WTLogoutRequest *msg);
    void handlePictureRequest(WTPictureRequest *msg);
    void handlePictureResponse(WTPictureResponse *msg);
    void handleSessionJoinRequest(WTSessionJoinRequest *msg);
    void handleSessionJoinResponse(WTSessionJoinResponse *msg);
    void handleSessionLeaveRequest(WTSessionLeaveRequest *msg);
    void handleSessionLeaveResponse(WTSessionLeaveResponse *msg);
    void handleSessionListRequest(WTSessionListRequest *msg);
    void handleSessionListResponse(WTSessionListResponse *msg);
    void handleSessionMemberUpdate(WTSessionMemberUpdate *msg);
    void handleUpdateDrawing(WTUpdateDrawing *msg);
    void handleWritePermissionRequest(WTWritePermissionRequest *msg);
    void handleWritePermissionStatus(WTWritePermissionStatus *msg);
    void handlePeerHandshake(WTPeerHandshake *msg);

signals:
    void sendMessage(QString destination, QByteArray data);
    void receivedLoginRequest(QString userName);
    void receivedLoginResponse(QString userName, QChar result, QString infoMsg);
    void receivedLogoutRequest(QString userName);
    void receivedPeerHandshake(QString userName, QString sessionName);
    void receivedPictureRequest(QString userName, QString sessionName);
    void receivedPictureResponse(QString userName, QString sessionName, QByteArray picData);
    void receivedSessionJoinRequest(QString userName, QString sessionName, QString password);
    void receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users);
    void receivedSessionLeaveRequest(QString userName, QString sessionName);
    void receivedSessionLeaveResponse(QString userName, QString sessionName, char result);
    void receivedSessionListRequest(QString userName);
    void receivedSessionListResponse(QString userName, QStringList sessionList);
    void receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QHash<QString, long> users);
    void receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionRequest(QString userName);
    void receivedWritePermissionStatus(QString userName, char status);

public slots:
    void receiveMessage(QString origin, QByteArray data);

    void sendLoginRequest(QString destUserName);
    void sendLoginResponse(QString destUserName, char result, QString infoMsg);
    void sendLogoutRequest(QString destUserName);
    void sendPeerHandshake(QString destUserName, QString sessionName);
    void sendPictureRequest(QString destUserName, QString sessionName);
    void sendPictureResponse(QString destUserName, QString sessionName, QByteArray picData);
    void sendSessionJoinRequest(QString destUserName, QString sessionName, QString password);
    void sendSessionJoinResponse(QString destUserName, QString sessionName, char result, QHash<QString, long> users);
    void sendSessionLeaveRequest(QString destUserName, QString sessionName);
    void sendSessionLeaveResponse(QString destUserName, QString sessionName, char result);
    void sendSessionListRequest(QString destUserName);
    void sendSessionListResponse(QString destUserName, QStringList sessionList);
    void sendSessionMemberUpdate(QString destUserName, QString sessionName, char updateType, QHash<QString, long> users);
    void sendUpdateDrawing(QString destUserName, QString sessionName, QByteArray picData);
    void sendWritePermissionRequest(QString destUserName);
    void sendWritePermissionStatus(QString destUserName, char status);
};

#endif // PROTOCOLHANDLER_H
