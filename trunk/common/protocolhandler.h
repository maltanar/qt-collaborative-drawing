#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <messagetransceiver.h>

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);
    virtual void receiveData(QString origin, QByteArray data);
};

#endif // PROTOCOLHANDLER_H
