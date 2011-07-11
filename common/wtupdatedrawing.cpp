#include "wtupdatedrawing.h"

WTUpdateDrawing::WTUpdateDrawing() :
    WTMessage()
{
    command = "DRAWUPDT";
}

QByteArray WTUpdateDrawing::serialize()
{
    msgSize += 8 + picData.size();
    WTMessage::serialize();
    m_serializer << sessionName << picData;

    return m_serializedData;
}

void WTUpdateDrawing::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> picData;
}

QString WTUpdateDrawing::getSessionName()
{
    return this->sessionName;
}

void WTUpdateDrawing::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QByteArray WTUpdateDrawing::getPicData()
{
    return picData;
}

void WTUpdateDrawing::setPicData(QByteArray picData)
{
    this->picData = picData;
}

