#ifndef WTPICTUREREQUEST_H
#define WTPICTUREREQUEST_H

#include <wtmessage.h>

class WTPictureRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTPictureRequest(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    QString getSessionName();
    void setSessionName(QString sessionName);

private:
    QString sessionName;

};

#endif // WTPICTUREREQUEST_H
