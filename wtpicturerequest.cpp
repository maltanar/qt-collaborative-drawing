#include "wtpicturerequest.h"

WTPictureRequest::WTPictureRequest(QObject *parent) :
    WTMessage(parent)
{
    command = "COLSTARQ";
}

QByteArray WTPictureRequest::serialize()
{
    //Size of sessionName
    msgSize += 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    return data;
}

void WTPictureRequest::deserialize(QByteArray *data)
{
    char sessionName[9];
    QDataStream dataStream(*data);
    WTMessage::deserialize(data);
    dataStream.skipRawData(24);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
}

QString WTPictureRequest::getSessionName()
{
    return this->sessionName;
}

void WTPictureRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
