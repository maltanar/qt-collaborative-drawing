#include "wtwritepermissionstatus.h"

WTWritePermissionStatus::WTWritePermissionStatus() :
    WTMessage()
{
    command = "WRTPRMST";
}

QByteArray WTWritePermissionStatus::serialize()
{
    //Size of status
    msgSize += 1;
    WTMessage::serialize();
    m_serializer << status;

    return m_serializedData;
}

void WTWritePermissionStatus::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> status;
}

QChar WTWritePermissionStatus::getStatus()
{
    return this->status;
}

void WTWritePermissionStatus::setStatus(QChar status)
{
    this->status = status;
}
