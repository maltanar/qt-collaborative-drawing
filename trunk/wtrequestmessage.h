#ifndef WTREQUESTMESSAGE_H
#define WTREQUESTMESSAGE_H

#include "wtmessage.h"

class WTRequestMessage : public WTMessage
{
    Q_OBJECT
public:
    explicit WTRequestMessage(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);
    // username is common for all request messages
    void setUsername(QString username);
    QString getUsername();

private:
    QString username;

};

#endif // WTREQUESTMESSAGE_H
