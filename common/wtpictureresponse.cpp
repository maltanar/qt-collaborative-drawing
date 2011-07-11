#include "wtpictureresponse.h"

WTPictureResponse::WTPictureResponse() :
    WTMessage()
{
    command = "COLSTARS";
}

QByteArray WTPictureResponse::serialize()
{
    //size of sessionName + size of picdata
    msgSize += 8 + picData.size();
    WTMessage::serialize();
    m_serializer << sessionName << picData;

    return m_serializedData;
}

void WTPictureResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);

    m_serializer >> sessionName >> picData;
}

QString WTPictureResponse::getSessionName()
{
    return this->sessionName;
}

void WTPictureResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QByteArray WTPictureResponse::getPicData()
{
    return picData;
}

void WTPictureResponse::setPicData(QByteArray picData)
{
    this->picData = picData;
}
