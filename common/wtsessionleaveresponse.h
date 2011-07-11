#ifndef WTSESSIONLEAVERESPONSE_H
#define WTSESSIONLEAVERESPONSE_H

#include <wtmessage.h>

class WTSessionLeaveResponse : public WTMessage
{
public:
    explicit WTSessionLeaveResponse();

    void deserialize(QByteArray data);
    QByteArray serialize();

    QChar getResult();
    void setResult(QChar result);

    void setSessionName(QString sessionName);
    QString getSessionName();

private:
    QString sessionName;
    QChar result;

};

#endif // WTSESSIONLEAVERESPONSE_H
