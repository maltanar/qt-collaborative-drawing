#ifndef WTSESSIONLISTREQUEST_H
#define WTSESSIONLISTREQUEST_H

#include <wtmessage.h>

class WTSessionListRequest : public WTMessage
{
public:
    WTSessionListRequest()
    {
        command = "SESSLREQ";
    }

    QByteArray serialize();
    void deserialize(QByteArray *data);
    void setUsername(QString username);
    QString getUsername();


private:
    QString username;
};

#endif // WTSESSIONLISTREQUEST_H
