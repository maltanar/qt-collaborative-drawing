#ifndef MESSAGETRANSCEIVER_H
#define MESSAGETRANSCEIVER_H

#include <QThread>
#include <QtNetwork>
#include <QHash>
#include <QString>

#include "wtmessage.h"

#define     COLLABORATION_TCP_PORT      45454

class MessageTransceiver : public QThread
{
    Q_OBJECT
public:
    explicit MessageTransceiver(QObject *parent = 0);
    void run();

    void addDestinationEntry(QString destination, QString hostname);
    QString resolveDestination(QString destination);
    void removeDestinationEntry(QString destination);

protected:
    QHash<QString, QTcpSocket *> mOpenConnections;
    QTcpServer * mServer;
    QHash<QString, QString> mDestinationEntries;

signals:

public slots:
    void connectTo(QString destination);
    void sendMessage(QString destination, WTMessage msg);

private slots:
    void newConnection();

};

#endif // MESSAGETRANSCEIVER_H
