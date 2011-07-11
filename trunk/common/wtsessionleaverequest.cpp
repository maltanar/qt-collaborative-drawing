#include "wtsessionleaverequest.h"

WTSessionLeaveRequest::WTSessionLeaveRequest() :
    WTMessage()
{
    command = "SESSQUIT";
}

QByteArray WTSessionLeaveRequest::serialize()
{
    //Size of sessionName
    msgSize += 8;
    WTMessage::serialize();
    m_serializer << sessionName;

    return m_serializedData;
}

void WTSessionLeaveRequest::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName;
}

QString WTSessionLeaveRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionLeaveRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
