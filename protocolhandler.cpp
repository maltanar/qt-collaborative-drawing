#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent)
{
}

void ProtocolHandler::receiveMessage(QString origin, QByteArray data)
{

}

bool ProtocolHandler::deliverMessage(WTMessage * msg)
{
    return true;
}

void ProtocolHandler::handleLoginRequest(WTLoginMessage *msg)
{

}

void ProtocolHandler::handleLoginResponse(WTLoginResponse *msg)
{

}

void ProtocolHandler::handleLogoutRequest(WTLogoutRequest *msg)
{

}

void ProtocolHandler::handlePictureRequest(WTPictureRequest *msg)
{

}

void ProtocolHandler::handlePictureResponse(WTPictureResponse *msg)
{

}

void ProtocolHandler::handleSessionJoinRequest(WTSessionJoinRequest *msg)
{

}

void ProtocolHandler::handleSessionJoinResponse(WTSessionJoinResponse *msg)
{

}

void ProtocolHandler::handleSessionLeaveRequest(WTSessionLeaveRequest *msg)
{

}

void ProtocolHandler::handleSessionLeaveResponse(WTSessionLeaveResponse *msg)
{

}

void ProtocolHandler::handleSessionListRequest(WTSessionListRequest *msg)
{

}

void ProtocolHandler::handleSessionListResponse(WTSessionListResponse *msg)
{

}

void ProtocolHandler::handleSessionMemberUpdate(WTSessionMemberUpdate *msg)
{

}

void ProtocolHandler::handleUpdateDrawing(WTUpdateDrawing *msg)
{

}

void ProtocolHandler::handleWritePermissionRequest(WTWritePermissionRequest *msg)
{

}

void ProtocolHandler::handleWritePermissionStatus(WTWritePermissionStatus *msg)
{

}
