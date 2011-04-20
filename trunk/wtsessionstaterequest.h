#ifndef WTSESSIONSTATEREQUEST_H
#define WTSESSIONSTATEREQUEST_H

#include <QString>
#include "wtrequestmessage.h"

class WTSessionStateRequest : public WTRequestMessage
{
    Q_OBJECT
public:
    explicit WTSessionStateRequest(QObject *parent = 0);

private:
    QString sessionName;

};

#endif // WTSESSIONSTATEREQUEST_H
