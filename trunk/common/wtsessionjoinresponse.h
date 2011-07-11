#ifndef WTSESSIONJOINRESPONSE_H
#define WTSESSIONJOINRESPONSE_H

#include <wtmessage.h>

class WTSessionJoinResponse : public WTMessage
{
public:
    explicit WTSessionJoinResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setResult(QChar result);
    QChar getResult();
    void setSessionName(QString sessionName);
    QString getSessionName();
    qint32 getUserCount();
    void addUser(QString username, qint32 userIP);
    qint32 getUserIP(QString username);
    void setUsers(QMap<QString, qint32> users);
    QMap<QString, qint32> getUsers();

private:
    QChar result;
    QString sessionName;
    qint32 userCount;
    QMap<QString, qint32> users;
};

#endif // WTSESSIONJOINRESPONSE_H
