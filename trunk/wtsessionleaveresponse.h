#ifndef WTSESSIONLEAVERESPONSE_H
#define WTSESSIONLEAVERESPONSE_H

#include <wtmessage.h>

class WTSessionLeaveResponse : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionLeaveResponse(QObject *parent = 0);

    void deserialize(QByteArray *data);
    QByteArray serialize();

    char getResult();
    void setResult(char result);

    void setSessionName(QString sessionName);
    QString getSessionName();

private:
    QString sessionName;
    char result;

};

#endif // WTSESSIONLEAVERESPONSE_H
