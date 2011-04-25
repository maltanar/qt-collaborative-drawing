#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QHash>

#include "wtmessage.h"

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);

private:
    QHash<QString, QString> peerMap;
    QHash<QString, QString> pendingMapRequests;

    bool deliverMessage(WTMessage * msg);

signals:
    void sendMessage(QString destination, QByteArray data);

public slots:
    void receiveMessage(QString origin, QByteArray data);

};

#endif // PROTOCOLHANDLER_H
