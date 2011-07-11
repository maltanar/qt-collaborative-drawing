#include "wtsessionlistresponse.h"

WTSessionListResponse::WTSessionListResponse() :
    WTMessage(),
    sesscnt(0)
{
    command = "SESSLRES";
}

QByteArray WTSessionListResponse::serialize()
{
    //size of sesscnt + total size of session names
    msgSize += 4 + sesscnt * 8;
    WTMessage::serialize();
    m_serializer << sesscnt << sessionList;

    return m_serializedData;
}

void WTSessionListResponse::deserialize(QByteArray data)
{
    WTMessage::deserialize(data);
    m_serializer >> sesscnt >> sessionList;
}

QStringList WTSessionListResponse::getSessionList()
{
    return sessionList;
}

void WTSessionListResponse::setSessionList(QStringList &sessionList)
{
    this->sessionList = sessionList;
    sesscnt = sessionList.count();
}

void WTSessionListResponse::addSession(QString sessionName)
{
    sesscnt++;
    sessionList.append(sessionName);
}



