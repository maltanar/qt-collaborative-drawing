#ifndef WTSESSIONMEMBERUPDATE_H
#define WTSESSIONMEMBERUPDATE_H

#include <wtmessage.h>

#define UPDATE_SESSION_JOIN_BEGIN  0
#define UPDATE_SESSION_JOIN_END    1
#define UPDATE_SESSION_LEAVE       2

class WTSessionMemberUpdate : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionMemberUpdate(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    uint getUpdatedMemberCount();

    char getUpdateType();
    void setUpdateType(char updateType);

    void addUser(QString username, long IP);
    QHash<QString, long> getUsers();
    void setUsers(QHash<QString, long> users);

    long getUserIP(QString username);
private:
    QString sessionName;
    uint updatedMemberCount;
    char updateType;
    QHash<QString, long> users;
};

#endif // WTSESSIONMEMBERUPDATE_H
