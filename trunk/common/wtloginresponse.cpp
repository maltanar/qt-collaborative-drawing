#include "wtloginresponse.h"

WTLoginResponse::WTLoginResponse() :
    WTMessage()
{
    command = "LOGINRES";
}

QByteArray WTLoginResponse::serialize()
{
    //Size of result + size of msg size + size of msg
    msgSize += 1 + 2 + infomsg.length();
    WTMessage::serialize();
    m_serializer << result << infomsgSize << infomsg;

    return m_serializedData;
}

void WTLoginResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);

    m_serializer >> result >> infomsgSize >> infomsg;

}

void WTLoginResponse::setInfomsg(QString msg)
{
    this->infomsg = msg;
    infomsgSize = msg.size();
}

QString WTLoginResponse::getInfomsg()
{
    return this->infomsg;
}

void WTLoginResponse::setResult(QChar result)
{
    this->result = result;
}

QChar WTLoginResponse::getResult()
{
    return this->result;
}
