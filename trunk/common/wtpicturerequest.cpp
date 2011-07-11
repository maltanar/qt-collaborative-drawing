#include "wtpicturerequest.h"

WTPictureRequest::WTPictureRequest() :
    WTMessage()
{
    command = "COLSTARQ";
}

QByteArray WTPictureRequest::serialize()
{
    //Size of sessionName
    msgSize += 8;
    WTMessage::serialize();
    m_serializer << sessionName;

    return m_serializedData;
}

void WTPictureRequest::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName;
}

QString WTPictureRequest::getSessionName()
{
    return this->sessionName;
}

void WTPictureRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

