#ifndef MESSAGETRANSCEIVER_H
#define MESSAGETRANSCEIVER_H

#include <QThread>
#include <QtNetwork>
#include <QHash>
#include <QString>
#include <QByteArray>

#define     TRANSCEIVER_TCP_PORT      45454
#define     TRANSCEIVER_HEADER        "MSGTXRX1"
#define     TIMEOUT                   60

class MessageTransceiver : public QThread
{
    Q_OBJECT
public:
    explicit MessageTransceiver(QObject *parent = 0);
    void run();

protected:
    QHash<QString, QTcpSocket *> mOpenConnections;
    QTcpServer * mServer;
    QHash<QString, QByteArray> originBuffers;
    QHash<QString, QByteArray> destBuffers;
    QHash<QString, unsigned int> originExpectedDataSize;
    QHash<QString, bool> mAliveConnections;
    QHash<QString, int> mTimeouts;

    void sendMessageNoHeader(QTcpSocket*,QByteArray msg);
    void processOriginBuffer(QString origin);

    void clearKeepAlives();

signals:
    void gotNewData(QString origin, QByteArray data);
    void clientDisconnected(QString clientIpAddress);

public slots:
    void connectTo(QString destination);
    void sendMessage(QString destination, QByteArray msg);

private slots:
    void newConnection();
    void connected();
    void disconnected();
    void dataArrived();
    void sendKeepAlive();
    void socketError(QAbstractSocket::SocketError err);

};

#endif // MESSAGETRANSCEIVER_H
