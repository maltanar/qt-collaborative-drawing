#ifndef WTSESSIONJOINREQUEST_H
#define WTSESSIONJOINREQUEST_H

#include <wtmessage.h>
#include <QCryptographicHash>


class WTSessionJoinRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionJoinRequest(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setSessionName(QString sessionName);
    QString getSessionName();

    void setPassword(QString password);
    QString getPassword();

private:
    QString sessionName;
    QString password;

};

#endif // WTSESSIONJOINREQUEST_H
