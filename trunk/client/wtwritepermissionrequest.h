#ifndef WTWRITEPERMISSIONREQUEST_H
#define WTWRITEPERMISSIONREQUEST_H

#include <wtmessage.h>

class WTWritePermissionRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTWritePermissionRequest(QObject *parent = 0);

};

#endif // WTWRITEPERMISSIONREQUEST_H
