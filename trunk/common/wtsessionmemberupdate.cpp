#include "wtsessionmemberupdate.h"

WTSessionMemberUpdate::WTSessionMemberUpdate() :
    WTMessage()
{
    command = "SESSMUPD";
}

QByteArray WTSessionMemberUpdate::serialize()
{
    //Size of sessionName + size of updatedMemberCount + size of updateType
    // + total size of members' usernames and IPs
    msgSize += 8 + 4 + 1 + 8;
    WTMessage::serialize();
    m_serializer << sessionName << updateType << user;

    return m_serializedData;
}

void WTSessionMemberUpdate::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sessionName >> updateType >> user;
}

QString WTSessionMemberUpdate::getSessionName()
{
    return this->sessionName;
}

void WTSessionMemberUpdate::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}


void WTSessionMemberUpdate::setUpdateType(QChar updateType)
{
    this->updateType = updateType;
}

QChar WTSessionMemberUpdate::getUpdateType()
{
    return this->updateType;
}

QString WTSessionMemberUpdate::getUser()
{
    return user;
}

void WTSessionMemberUpdate::setUser(QString user)
{
    this->user = user;
}
