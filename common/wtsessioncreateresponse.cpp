#include "wtsessioncreateresponse.h"

WTSessionCreateResponse::WTSessionCreateResponse() :
    WTMessage()
{
        command = "SESCRTRS";
}


QByteArray WTSessionCreateResponse::serialize()
{
    //Size of common header + result + session name + encrypted password
    msgSize += 8 + 1 + 32;
    QByteArray data = WTMessage::serialize();
    m_serializer << result << sessionName << password;
    return m_serializedData;
}

void WTSessionCreateResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> result >> sessionName >> password;
}

QString WTSessionCreateResponse::getSessionName()
{
    return this->sessionName;
}

void WTSessionCreateResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QChar WTSessionCreateResponse::getResult()
{
    return this->result;
}

void WTSessionCreateResponse::setResult(QChar result)
{
    this->result = result;
}

QString WTSessionCreateResponse::getPassword()
{
    return this->password;
}

void WTSessionCreateResponse::setPassword(QString password)
{
    //It is assumed here that the password is already encrypted
    this->password = password;
}
