#ifndef WTSESSIONMEMBERUPDATE_H
#define WTSESSIONMEMBERUPDATE_H

#include <wtmessage.h>

class WTSessionMemberUpdate : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionMemberUpdate(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    uint getUpdatedMemberCount();

    char getUpdateType();
    void setUpdateType(char updateType);

    void addUser(QString username, long IP);
    QHash<QString, long> getUsers();

    long getUserIP(QString username);
private:
    QString sessionName;
    uint updatedMemberCount;
    char updateType;
    QHash<QString, long> users;
};

#endif // WTSESSIONMEMBERUPDATE_H
