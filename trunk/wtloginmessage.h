#ifndef WTLOGINMESSAGE_H
#define WTLOGINMESSAGE_H

#include <wtmessage.h>

class WTLoginMessage : public WTMessage
{
    Q_OBJECT

public:
    WTLoginMessage()
    {
        command = "LOGINREQ";
    }
    ~WTLoginMessage() {}

    QByteArray serialize();
    void deserialize(QByteArray *data);
    void setUsername(QString username);
    QString getUsername();

private:
    QString username;

};

#endif // WTLOGINMESSAGE_H
