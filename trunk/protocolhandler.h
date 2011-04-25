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

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);

private:
    QHash<QString, QString> peerMap;
    QHash<QString, QString> pendingMapRequests;

    bool deliverMessage(WTMessage * msg);

    void handleMapRequestStatus(QString username, bool confirmed, QString destination = "");

    void handleLoginRequest(WTLoginMessage *msg);
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

signals:
    void sendMessage(QString destination, QByteArray data);

public slots:
    void receiveMessage(QString origin, QByteArray data);

};

#endif // PROTOCOLHANDLER_H
