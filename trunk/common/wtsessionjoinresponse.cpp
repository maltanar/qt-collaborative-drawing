#include "wtsessionjoinresponse.h"

WTSessionJoinResponse::WTSessionJoinResponse() :
    WTMessage(),
    userCount(0)
{
        command = "SESSRESP";
}

QByteArray WTSessionJoinResponse::serialize()
{
    //Size of result + size of sessionname + size of usercount + size of usernames and ips
    msgSize += 1 + 8 + 4 + 12 * userCount;
    WTMessage::serialize();
    m_serializer << result << sessionName << userCount << users;

    return m_serializedData;
}

void WTSessionJoinResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> result >> sessionName >> userCount >> users;
}


void WTSessionJoinResponse::setResult(QChar result)
{
    this->result = result;
}

QChar WTSessionJoinResponse::getResult()
{
    return this->result;
}

void WTSessionJoinResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionJoinResponse::getSessionName()
{
    return this->sessionName;
}

qint32 WTSessionJoinResponse::getUserCount()
{
    return this->userCount;
}

void WTSessionJoinResponse::addUser(QString username, qint32 userIP)
{
    this->userCount++;
    users.insert(username, userIP);
}

qint32 WTSessionJoinResponse::getUserIP(QString username)
{
    QMap<QString,qint32>::const_iterator iter = users.find(username);
    return iter.value();
}

void WTSessionJoinResponse::setUsers(QMap<QString, qint32> users)
{
    this->users = users;
    this->userCount = users.size();
}

QMap<QString, qint32> WTSessionJoinResponse::getUsers()
{
    return this->users;
}

