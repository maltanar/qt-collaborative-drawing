#include "wtsessionjoinrequest.h"

WTSessionJoinRequest::WTSessionJoinRequest() :
    WTMessage()
{
    command = "SESSJOIN";
}

QByteArray WTSessionJoinRequest::serialize()
{
    //Size of sessionName + size of encrypted password
    msgSize += 8 + 32;
    WTMessage::serialize();
    m_serializer << sessionName << password;

    return m_serializedData;
}

void WTSessionJoinRequest::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> password;
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
    //Store the password as encrypted
    this->password = password;
    //this->password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5);
}

QString WTSessionJoinRequest::getPassword()
{
    return password;
}


