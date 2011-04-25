#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent)
{
}

// receiveMessage is responsible for handling byte array data coming
// from the MessageTransceiver by checking for proper message format,
// deserializing it, constructing the appropriate message object
// and passing it to its corresponding handler
void ProtocolHandler::receiveMessage(QString origin, QByteArray data)
{
    // check for zero-length message
    if(data.length() == 0) {
        // TODO what to do?
        qWarning() << "ProtocolHandler::receiveMessage got message with length" << data.length();
        return;
    }

    WTMessage candidateMsg;
    // TODO pass all byte arrays as non-pointers
    candidateMsg.deserialize(data);
    // check if deserialized data is of correct format!
    if(candidateMsg.getVersion() != "WTC1") {
        // incorrect or damaged message
        qWarning() <<  "ProtocolHandler::receiveMessage got message with incorrect header" << candidateMsg.getVersion();
        return;
    }
    // TODO determine the message type and deserialize accordingly

}

// deliverMessage is responsible for handing the given message in byte array
// form to the MessageTransceiver with the corresponding peer destination
// for this username
bool ProtocolHandler::deliverMessage(WTMessage * msg)
{
    // check for null messages
    if(!msg)
        return false;
    // find the peer destination for this username
    QString destination = peerMap.value(msg->getUsername(), "");
    if(destination == "") {
        // the destination address for this peer is not defined
        qWarning() << "deliverMessage: destination address for peer" << msg->getUsername() << "undefined";
        delete msg;
        return false;
    }
    // serialize the message into a byte array
    QByteArray msgData = msg->serialize();
    // time for delivery!
    emit sendMessage(destination, msgData);
    // we should be able to delete this message now
    // TODO handle the new - delete logic more carefully! what happens when errors
    // occur, for example?
    delete msg;
    return true;
}


void ProtocolHandler::handleMapRequestStatus(QString username, bool confirmed, QString destination)
{
    if(confirmed) {
        // this mapping has been confirmed
        // we don't need the destination parameter, look for the username
        // in the pending map requests and move it to the established mappings
        destination = pendingMapRequests.value(username, "");
        if(destination == "") {
            // oops - something went wrong
            // no such mapping request pending
            // TODO deeper error handling?
            qWarning() << "no pending mapping request found for confirming" << username;
            return;
        } else {
            // add this mapping to the list of established peer mappings
            peerMap.insert(username, destination);
        }
    }

    // remove from list of pending requests
    pendingMapRequests.remove(username);
}


void ProtocolHandler::handleLoginRequest(WTLoginMessage *msg, QString requestOrigin)
{
    // Local peer role: Server
    // add a mapping request for the remote peer
    // this will be verified or discarded upon login result
    pendingMapRequests.insert(msg->getUsername(), requestOrigin);

    emit receivedLoginRequest(msg->getUsername());
}

void ProtocolHandler::handleLoginResponse(WTLoginResponse *msg)
{
    // Local peer role: Client
    emit receivedLoginResponse(msg->getUsername(), msg->getResult(), msg->getInfomsg());
}

void ProtocolHandler::handleLogoutRequest(WTLogoutRequest *msg)
{
    // Local peer role: Server
    emit receivedLogoutRequest(msg->getUsername());
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
