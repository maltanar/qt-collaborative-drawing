#include "wtsessionlistrequest.h"

QByteArray WTSessionListRequest::serialize()
{
    msgSize += 8; //size of username
    QByteArray data = WTMessage::serialize();
    //TODO pad the username to 8 characters
    data.append(username.toAscii());
    return data;
}

void WTSessionListRequest::deserialize(QByteArray *data)
{
    WTMessage::deserialize(data);
    username = QString(data.right(8));
}

void WTSessionListRequest::setUsername(QString username)
{
    this->username = username;
}

QString WTSessionListRequest::getUsername()
{
    return this->username;
}
