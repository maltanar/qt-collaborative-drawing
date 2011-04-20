#ifndef WTLOGINMESSAGE_H
#define WTLOGINMESSAGE_H

#include "wtrequestmessage.h"

class WTLoginMessage : public WTRequestMessage
{
    Q_OBJECT
public:
    explicit WTLoginMessage(QObject *parent = 0);

signals:

public slots:

};

#endif // WTLOGINMESSAGE_H
