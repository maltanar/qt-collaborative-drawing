#ifndef WTSESSIONMEMBERUPDATE_H
#define WTSESSIONMEMBERUPDATE_H

#include <wtmessage.h>

#define UPDATE_SESSION_JOIN_BEGIN  0
#define UPDATE_SESSION_JOIN_END    1
#define UPDATE_SESSION_LEAVE       2

class WTSessionMemberUpdate : public WTMessage
{
public:
    explicit WTSessionMemberUpdate();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QChar getUpdateType();
    void setUpdateType(QChar updateType);

    QString getUser();
    void setUser(QString user);

private:
    QString sessionName;
    QChar updateType;
    QString user;
};

#endif // WTSESSIONMEMBERUPDATE_H
