#include "wtsessionjoinrequest.h"

WTSessionJoinRequest::WTSessionJoinRequest(QObject *parent) :
    WTMessage(parent)
{
    command = "SESSJOIN";
}

QByteArray WTSessionJoinRequest::serialize()
{
    //Size of sessionName + size of password
    msgSize += 8 + 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8,' ').toAscii());
    //TODO Encrypt password
    data.append(password.leftJustified(8, ' ').toAscii());
    return data;
}

void WTSessionJoinRequest::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    char password[9];
    char sessionName[9];
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    //TODO size has to be changed to size of encrypted password
    dataStream.readRawData(password, 8);
    sessionName[8] = '\0';
    password[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    this->password = QString(password).trimmed();
}

void WTSessionJoinRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionJoinRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionJoinRequest::setPassword(QString password)
{
    this->password = password;
}

QString WTSessionJoinRequest::getPassword()
{
    return password;
}


