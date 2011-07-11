#ifndef SHAREDCANVASPROTOCOLHANDLER_H
#define SHAREDCANVASPROTOCOLHANDLER_H

#include <QObject>
#include <QMap>

#include "wtmessage.h"
#include <wtloginmessage.h>
#include <wtloginresponse.h>
#include <wtlogoutrequest.h>
#include <wtpicturerequest.h>
#include <wtpictureresponse.h>
#include <wtsessioncreaterequest.h>
#include <wtsessioncreateresponse.h>
#include <wtsessionjoinrequest.h>
#include <wtsessionjoinresponse.h>
#include <wtsessionleaverequest.h>
#include <wtsessionleaveresponse.h>
#include <wtsessionlistrequest.h>
#include <wtsessionlistresponse.h>
#include <wtsessionmemberupdate.h>
#include <wtupdatedrawing.h>
#include <wtupdatedrawingserver.h>
#include <wtwritepermissionrequest.h>
#include <wtwritepermissionstatus.h>
#include <wtpeerhandshake.h>
#include <protocolhandler.h>
#include <messagedispatcher.h>

#include "messagetransceiver.h"

class SharedCanvasProtocolHandler : public ProtocolHandler
{
    Q_OBJECT
public:
    explicit SharedCanvasProtocolHandler();

    void addUserMapping(QString userName, QString IP);
    QString getUserMapping(QString userName);

    void setMessageTransceiver(MessageTransceiver * newMesssageTransceiver);
    MessageTransceiver* getMessageTransceiver();

    void setMessageDispatcher(MessageDispatcher * messageDispatcher);
    MessageDispatcher* getMessageDispatcher();

    QString getUserName();
    void setUserName(QString username);

private:
    MessageTransceiver * m_messageTransceiver;
    MessageDispatcher * m_messageDispatcher;
    QMap<QString, QString> peerMap;
    QMap<QString, QString> pendingMapRequests;
    QString userName;

    bool deliverMessage(WTMessage * msg);

    void handleMapRequestStatus(QString username, bool confirmed);

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
    void handleUpdateDrawingServer(WTUpdateDrawingServer *msg);
    void handleWritePermissionRequest(WTWritePermissionRequest *msg);
    void handleWritePermissionStatus(WTWritePermissionStatus *msg);
    void handlePeerHandshake(WTPeerHandshake *msg, QString requestOrigin);
    void handleSessionCreateRequest(WTSessionCreateRequest *msg);
    void handleSessionCreateResponse(WTSessionCreateResponse *msg);


signals:
    void sendMessage(QString destination, QByteArray data);
    void sendMessageLoopback(QString desination, QByteArray data);
    void memberDisconnected(QString username);

    void receivedLoginRequest(QString userName);
    void receivedLoginResponse(QString userName, QChar result, QString infoMsg);
    void receivedLogoutRequest(QString userName);
    void receivedPeerHandshake(QString userName, QString sessionName);
    void receivedPictureRequest(QString userName, QString sessionName);
    void receivedPictureResponse(QString userName, QString sessionName, QByteArray picData);
    void receivedSessionJoinRequest(QString userName, QString sessionName, QString password);
    void receivedSessionJoinResponse(QString userName, QString sessionName, QChar result, qint32 userCount, QMap<QString, qint32> users);
    void receivedSessionLeaveRequest(QString userName, QString sessionName);
    void receivedSessionLeaveResponse(QString userName, QString sessionName, QChar result);
    void receivedSessionListRequest(QString userName);
    void receivedSessionListResponse(QString userName, QStringList sessionList);
    void receivedSessionMemberUpdate(QString userName, QString sessionName, QChar updateType, QString user);
    void receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData);
    void receivedUpdateDrawingServer(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionRequest(QString userName);
    void receivedWritePermissionStatus(QString userName, QChar status);
    void receivedSessionCreateRequest(QString userName, QString sessionName, QString password);
    void receivedSessionCreateResponse(QString userName, QString sessionName, QChar result, QString password);

public slots:
    void receiveData(QString origin, QByteArray data);
    void clientDisconnected(QString origin);

    void sendLoginRequest(QString destUserName);
    void sendLoginResponse(QString destUserName, QChar result, QString infoMsg);
    void sendLogoutRequest(QString destUserName);
    void sendPeerHandshake(QString destUserName, QString sessionName);
    void sendPictureRequest(QString destUserName, QString sessionName);
    void sendPictureResponse(QString destUserName, QString sessionName, QByteArray picData);
    void sendSessionJoinRequest(QString destUserName, QString sessionName, QString password);
    void sendSessionJoinResponse(QString destUserName, QString sessionName, QChar result, QMap<QString, qint32> users);
    void sendSessionLeaveRequest(QString destUserName, QString sessionName);
    void sendSessionLeaveResponse(QString destUserName, QString sessionName, QChar result);
    void sendSessionListRequest(QString destUserName);
    void sendSessionListResponse(QString destUserName, QStringList sessionList);
    void sendSessionMemberUpdate(QString destUserName, QString sessionName, QChar updateType, QString users);
    void sendUpdateDrawing(QString destUserName, QString sessionName, QByteArray picData);
    void sendUpdateDrawingServer(QString destUserName, QString sessionName, QByteArray picData);
    void sendWritePermissionRequest(QString destUserName);
    void sendWritePermissionStatus(QString destUserName, QChar status);
    void sendSessionCreateRequest(QString destUserName, QString sessionName, QString password);
    void sendSessionCreateResponse(QString destUserName, QString sessionName, QChar result, QString password);
};

#endif // SHAREDCANVASPROTOCOLHANDLER_H
