#include "wtwritepermissionstatus.h"

WTWritePermissionStatus::WTWritePermissionStatus(QObject *parent) :
    WTMessage(parent)
{
    command = "WRTPRMST";
}

QByteArray WTWritePermissionStatus::serialize()
{
    //Size of status
    msgSize += 1;
    QByteArray data = WTMessage::serialize();
    data.append(status);
    return data;
}

void WTWritePermissionStatus::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(24);
    dataStream.readRawData(&status, 1);
}

char WTWritePermissionStatus::getStatus()
{
    return this->status;
}

void WTWritePermissionStatus::setStatus(char status)
{
    this->status = status;
}
