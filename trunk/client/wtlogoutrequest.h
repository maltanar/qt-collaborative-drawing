#ifndef WTLOGOUTREQUEST_H
#define WTLOGOUTREQUEST_H

#include <wtmessage.h>

class WTLogoutRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTLogoutRequest(QObject *parent = 0);

};

#endif // WTLOGOUTREQUEST_H
