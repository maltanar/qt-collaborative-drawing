#include "wtmessage.h"

WTMessage::WTMessage(QObject *parent) :
        QObject(parent), msgSize(24) //Constant header size + username
{
        version = "WTC1";
}

WTMessage::~WTMessage()
{

}

QByteArray WTMessage::serialize()
{
    QByteArray data(version.toAscii());
    data.append(command.toAscii());
    data.append(QByteArray::fromRawData((const char*)&msgSize, 4));
    data.append(username.leftJustified(8, ' ').toAscii());
    return data;
}

void WTMessage::deserialize(QByteArray data)
{
    //Assuming the header is always of the same size.
    char version[5];
    char command[9];
    char username[9];
    QDataStream dataStream(data);
    dataStream.readRawData(version, 4);
    version[4] = '\0';
    dataStream.readRawData(command, 8);
    command[8] = '\0';
    dataStream.readRawData((char *)&msgSize, 4);
    dataStream.readRawData(username, 8);
    username[8] = '\0';
    this->version = QString(version);
    this->command = QString(command);
    this->username = QString(username).trimmed();
}


void WTMessage::setUsername(QString username)
{
    this->username = username;
}

QString WTMessage::getUsername()
{
    return this->username;
}

void WTMessage::setCommand(QString command)
{
    this->command = command;
}

QString WTMessage::getCommand()
{
    return this->command;
}

void WTMessage::setVersion(QString version)
{
    this->version = version;
}

QString WTMessage::getVersion()
{
    return this->version;
}
