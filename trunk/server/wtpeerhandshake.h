#ifndef WTPEERHANDSHAKE_H
#define WTPEERHANDSHAKE_H

#include <wtmessage.h>

class WTPeerHandshake : public WTMessage
{
    Q_OBJECT
public:
    explicit WTPeerHandshake(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);
private:
    QString sessionName;

};

#endif // WTPEERHANDSHAKE_H
