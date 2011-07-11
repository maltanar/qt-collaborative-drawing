#include "wtsessioncreaterequest.h"


WTSessionCreateRequest::WTSessionCreateRequest() :
    WTMessage()
{
    command = "SESCRTRQ";
}


QByteArray WTSessionCreateRequest::serialize()
{
    //Message size is the size of common header + session name + password in md5
    msgSize += 8 + 32;
    WTMessage::serialize();
    m_serializer << sessionName << password;

    return m_serializedData;
}

void WTSessionCreateRequest::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> password;
}

QString WTSessionCreateRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionCreateRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionCreateRequest::getPassword()
{
    return this->password;
}

void WTSessionCreateRequest::setPassword(QString password)
{
    //Convert the password into md5
    this->password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5);
}
