#ifndef WTLOGINRESPONSEMESSAGE_H
#define WTLOGINRESPONSEMESSAGE_H

#include <wtmessage.h>

class WTLoginResponseMessage : public WTMessage
{
    Q_OBJECT

public:
    WTLoginResponseMessage()
    {
        command = "LOGINRES";
    }

    QByteArray serialize();
    void deserialize(QByteArray *data);

    void setInfomsg(QString msg);
    QString getInfomsg();

    void setResult(QChar result);
    QChar getResult();

private:
    QChar result;
    QString infomsg;
    int infomsgSize;

};

#endif // WTLOGINRESPONSEMESSAGE_H
