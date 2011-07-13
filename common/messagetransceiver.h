#ifndef MESSAGETRANSCEIVER_H
#define MESSAGETRANSCEIVER_H

#include <QThread>
#include <QtNetwork>
#include <QMap>
#include <QString>
#include <QByteArray>

#define     TRANSCEIVER_TCP_PORT      45454
#define     TRANSCEIVER_HEADER        "MSGTXRX1"
#define     TIMEOUT                   20    // 20 * 3000 milliseconds = 60 seconds
#define     KEEP_ALIVE_PERIOD      3000 //Milliseconds

class MessageTransceiver : public QObject
{
    Q_OBJECT
public:
    explicit MessageTransceiver(QObject *parent = 0);
    void run();

protected:
    QMap<QString, QTcpSocket *> mOpenConnections;
    QTcpServer * mServer;
    QMap<QString, QByteArray> originBuffers;
    QMap<QString, QByteArray> destBuffers;
    QMap<QString, qint32> originExpectedDataSize;
    QMap<QString, bool> mAliveConnections;
    QMap<QString, int> mTimeouts;

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
    void disconnectPeer(QString peerAddress);

};

#endif // MESSAGETRANSCEIVER_H
