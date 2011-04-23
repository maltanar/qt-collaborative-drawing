#ifndef WTSESSIONLEAVEREQUEST_H
#define WTSESSIONLEAVEREQUEST_H

#include <wtmessage.h>

class WTSessionLeaveRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionLeaveRequest(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    void setSessionName(QString sessionName);
    QString getSessionName();

private:
    QString sessionName;

};

#endif // WTSESSIONLEAVEREQUEST_H
