#ifndef WTLOGINRESPONSE_H
#define WTLOGINRESPONSE_H

#include <wtmessage.h>

class WTLoginResponse : public WTMessage
{
    Q_OBJECT
public:
    explicit WTLoginResponse(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    void setInfomsg(QString msg);
    QString getInfomsg();

    void setResult(char result);
    char getResult();

private:
    char result;
    QString infomsg;
    short infomsgSize;
};

#endif // WTLOGINRESPONSE_H
