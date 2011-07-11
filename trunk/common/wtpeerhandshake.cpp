#include "wtpeerhandshake.h"

WTPeerHandshake::WTPeerHandshake() :
    WTMessage()
{
    command = "HELLOPAL";
}


QByteArray WTPeerHandshake::serialize()
{
    //Size of sessionName
    msgSize += 8;
    WTMessage::serialize();
    m_serializer << sessionName;

    return m_serializedData;
}


void WTPeerHandshake::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName;
}

QString WTPeerHandshake::getSessionName()
{
    return this->sessionName;
}

void WTPeerHandshake::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
