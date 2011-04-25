#include "wtupdatedrawing.h"

WTUpdateDrawing::WTUpdateDrawing(QObject *parent) :
    WTMessage(parent)
{
    command = "DRAWUPDT";
}

QByteArray WTUpdateDrawing::serialize()
{
    msgSize += 8 + picData.size();
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(picData);
    return data;
}

void WTUpdateDrawing::deserialize(QByteArray data)
{
    char sessionName[9];
    char *picData;
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(24);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    //Rest of the characters belong to picture data
    picData = new char[data.size()-32];
    dataStream.readRawData(picData, data.size() - 32);
    this->picData.setRawData(picData, data.size()-32);
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

