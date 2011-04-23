#ifndef WTSESSIONJOINRESPONSE_H
#define WTSESSIONJOINRESPONSE_H

#include <wtmessage.h>

class WTSessionJoinResponse : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionJoinResponse(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    void setResult(char result);
    char getResult();
    void setSessionName(QString sessionName);
    QString getSessionName();
    int getUserCount();
    void addUser(QString username, long userIP);
    long getUserIP(QString username);
    QHash<QString, long> getUserList();

private:
    char result;
    QString sessionName;
    int userCount;
    QHash<QString, long> users;
};

#endif // WTSESSIONJOINRESPONSE_H
