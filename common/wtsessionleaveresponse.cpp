#include "wtsessionleaveresponse.h"

WTSessionLeaveResponse::WTSessionLeaveResponse() :
    WTMessage()
{
    command = "SESSQACK";
}

QByteArray WTSessionLeaveResponse::serialize()
{
    //Size of sessionName + size of result
    msgSize += 8 + 1;
    WTMessage::serialize();
    m_serializer << sessionName << result;

    return m_serializedData;
}

void WTSessionLeaveResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> result;
}

QChar WTSessionLeaveResponse::getResult()
{
    return this->result;
}

void WTSessionLeaveResponse::setResult(QChar result)
{
    this->result = result;
}

QString WTSessionLeaveResponse::getSessionName()
{
    return this->sessionName;
}

void WTSessionLeaveResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
