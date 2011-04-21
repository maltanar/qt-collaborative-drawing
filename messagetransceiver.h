#ifndef MESSAGETRANSCEIVER_H
#define MESSAGETRANSCEIVER_H

#include <QThread>
#include <QtNetwork>
#include <QHash>
#include <QString>
#include <QByteArray>

#define     TRANSCEIVER_TCP_PORT      45454

class MessageTransceiver : public QThread
{
    Q_OBJECT
public:
    explicit MessageTransceiver(QObject *parent = 0);
    void run();

protected:
    QHash<QString, QTcpSocket *> mOpenConnections;
    QTcpServer * mServer;

signals:
    void newData(QString origin, QByteArray data);

public slots:
    void connectTo(QString destination);
    void sendMessage(QString destination, QByteArray msg);

private slots:
    void newConnection();
    void connected();
    void disconnected();
    void dataArrived();

};

#endif // MESSAGETRANSCEIVER_H
