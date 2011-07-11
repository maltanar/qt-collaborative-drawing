#ifndef WTLOGINRESPONSE_H
#define WTLOGINRESPONSE_H

#include <wtmessage.h>

class WTLoginResponse : public WTMessage
{
public:
    explicit WTLoginResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setInfomsg(QString msg);
    QString getInfomsg();

    void setResult(QChar result);
    QChar getResult();

private:
    QChar result;
    QString infomsg;
    short infomsgSize;
};

#endif // WTLOGINRESPONSE_H
