#include "wtupdatedrawingserver.h"

// exactly the same as WTUpdateDrawing but meant for the server

WTUpdateDrawingServer::WTUpdateDrawingServer() :
    WTMessage()
{
    command = "DRAWSUPD";
}

QByteArray WTUpdateDrawingServer::serialize()
{
    msgSize += 8 + picData.size();
    WTMessage::serialize();
    m_serializer << sessionName << picData;
    return m_serializedData;
}

void WTUpdateDrawingServer::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> picData;
}

QString WTUpdateDrawingServer::getSessionName()
{
    return this->sessionName;
}

void WTUpdateDrawingServer::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QByteArray WTUpdateDrawingServer::getPicData()
{
    return picData;
}

void WTUpdateDrawingServer::setPicData(QByteArray picData)
{
    this->picData = picData;
}

