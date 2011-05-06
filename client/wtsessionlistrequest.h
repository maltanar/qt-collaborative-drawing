#ifndef WTSESSIONLISTREQUEST_H
#define WTSESSIONLISTREQUEST_H

#include "wtmessage.h"

class WTSessionListRequest : public WTMessage
{
    Q_OBJECT
public:
    explicit WTSessionListRequest(QObject *parent = 0);

signals:

public slots:

};

#endif // WTSESSIONLISTREQUEST_H
