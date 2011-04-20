#include "wtloginresponsemessage.h"

QByteArray WTLoginResponseMessage::serialize()
{
    //Size of result + size of msg size + size of msg
    msgSize += 1 + 2 + infomsg.length();
    QByteArray data = WTMessage::serialize();
    data.append(result.toAscii());
    data.append(QByteArray::fromRawData((const char*)&infomsgSize,2));
    data.append(infomsg.toAscii());
    return data;
}

void WTLoginResponseMessage::deserialize(QByteArray *data)
{
    int infomsgSize;
    WTMessage::deserialize(data);
    //TODO QDataStream
    memcpy(&result, data->right(data->length()-16).left(1),1);
    memcpy(&infomsgSize, data->right(data->length()-17).left(2),2);
    infomsg = QString(data->right(data->length()-19).left(infomsgSize).constData());
}

void WTLoginResponseMessage::setInfomsg(QString msg)
{
    this->infomsg = msg;
    infomsgSize = msg.size();
}

QString WTLoginResponseMessage::getInfomsg()
{
    return this->infomsg;
}

void WTLoginResponseMessage::setResult(QChar result)
{
    this->result = result;
}

QChar WTLoginResponseMessage::getResult()
{
    return this->result;
}
