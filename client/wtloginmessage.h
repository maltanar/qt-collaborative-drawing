#ifndef WTLOGINMESSAGE_H
#define WTLOGINMESSAGE_H

#include "wtmessage.h"

class WTLoginMessage : public WTMessage
{
    Q_OBJECT
public:
    explicit WTLoginMessage(QObject *parent = 0);

signals:

public slots:

};

#endif // WTLOGINMESSAGE_H
