#ifndef WTWRITEPERMISSIONSTATUS_H
#define WTWRITEPERMISSIONSTATUS_H

#include <wtmessage.h>

class WTWritePermissionStatus : public WTMessage
{
public:
    explicit WTWritePermissionStatus();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QChar getStatus();
    void setStatus(QChar status);

private:
    QChar status;
};

#endif // WTWRITEPERMISSIONSTATUS_H
