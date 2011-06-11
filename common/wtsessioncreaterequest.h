#ifndef WTSESSIONCREATEREQUEST_H
#define WTSESSIONCREATEREQUEST_H

#include <wtmessage.h>
#include <QCryptographicHash>

class WTSessionCreateRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionCreateRequest(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QString getPassword();
    void setPassword(QString password);

private:
    QString sessionName;
    QString password;

};

#endif // WTSESSIONCREATEREQUEST_H
