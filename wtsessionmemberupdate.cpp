#include "wtsessionmemberupdate.h"

WTSessionMemberUpdate::WTSessionMemberUpdate(QObject *parent) :
    WTMessage(parent),
    updatedMemberCount(0)
{
    command = "SESSMUPD";
}

QByteArray WTSessionMemberUpdate::serialize()
{
    //Size of sessionName + size of updatedMemberCount + size of updateType
    // + total size of members' usernames and IPs
    msgSize += 8 + 4 + 1 + updatedMemberCount*( 8 + 4 );
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(QByteArray::fromRawData((const char *)&updatedMemberCount, 4));
    data.append(updateType);
    QHash<QString, long>::iterator iter;
    for (iter = users.begin(); iter != users.end(); iter++)
    {
        data.append(((QString) iter.key()).leftJustified(8, ' ').toAscii());
        data.append(QByteArray::fromRawData((const char*)&(iter.value()),4));
    }
    return data;
}

void WTSessionMemberUpdate::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    dataStream.readRawData((char *)&updatedMemberCount, 4);
    dataStream.readRawData(&updateType, 1);

    for (unsigned int i = 0; i < updatedMemberCount; i++)
    {
        char username[9];
        long IP;
        dataStream.readRawData(username, 8);
        username[8] = '\0';
        dataStream.readRawData((char *)&IP, 4);
        users.insert(QString(username).trimmed(), IP);
    }
}

QString WTSessionMemberUpdate::getSessionName()
{
    return this->sessionName;
}

void WTSessionMemberUpdate::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

uint WTSessionMemberUpdate::getUpdatedMemberCount()
{
    return this->updatedMemberCount;
}

void WTSessionMemberUpdate::setUpdateType(char updateType)
{
    this->updateType = updateType;
}

char WTSessionMemberUpdate::getUpdateType()
{
    return this->updateType;
}

void WTSessionMemberUpdate::addUser(QString username, long IP)
{
    updatedMemberCount++;
    users.insert(username, IP);
}

QHash<QString, long> WTSessionMemberUpdate::getUsers()
{
    return users;
}

void WTSessionMemberUpdate::setUsers(QHash<QString, long> users)
{
    this->users = users;
}

long WTSessionMemberUpdate::getUserIP(QString username)
{
    return users.find(username).value();
}
