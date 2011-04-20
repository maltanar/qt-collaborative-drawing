#include "wtloginmessage.h"

QByteArray WTLoginMessage::serialize()
{
    msgSize += 8; //size of username
    QByteArray data = WTMessage::serialize();
    //TODO pad the username to 8 characters
    data.append(username.toAscii());
    return data;
}

void WTLoginMessage::deserialize(QByteArray *data)
{
    WTMessage::deserialize(data);
    username = QString(data->right(8));
}

void WTLoginMessage::setUsername(QString username)
{
    this->username = username;
}

QString WTLoginMessage::getUsername()
{
    return this->username;
}
