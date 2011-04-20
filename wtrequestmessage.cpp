#include "wtrequestmessage.h"

WTRequestMessage::WTRequestMessage(QObject *parent) :
    WTMessage(parent)
{
}

void WTRequestMessage::setUsername(QString username)
{
    this->username = username;
}

QString WTRequestMessage::getUsername()
{
    return this->username;
}

QByteArray WTRequestMessage::serialize()
{
    msgSize += 8; //size of username
    QByteArray data = WTMessage::serialize();
    data.append(username.leftJustified(8, ' ').toAscii());
    return data;
}

void WTRequestMessage::deserialize(QByteArray *data)
{
    WTMessage::deserialize(data);
    username = QString(data->right(8));
}
