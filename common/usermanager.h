#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QBuffer>
#include <QDebug>
#include <QTimer>
#include <QUdpSocket>

#include "messagetransceiver.h"
#include "protocolhandler.h"

#define USER_HANDSHAKE_SIGNATURE        QString("EKTHS")

// the period in ms with which UDP discovery messages from this client are broadcast
#define DISCOVERY_BROADCAST_PERIOD      1000
#define DISCOVERY_BROADCAST_SIGNATURE   QString("EKTUSERDISCOVERY")
#define DISCOVERY_BROADCAST_PORT        45460

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = 0);

    void setUserName(QString userName);
    QString getUserName();

    QMap<QString, QStringList> getAvailableUsers();
    QMap<QString, QString> getConnectedUsers();

    QString peerAddress(QString userName);

    void requestConnection(QString userName);

    MessageTransceiver * getMessageTransceiver();
    void setMessageTransceiver(MessageTransceiver * messageTransceiver);

    bool isDiscoveryBroadcastActive();

    void setDiscoveryBroadcastActive(bool active);

protected:
    void sendHandshake(QString destIp);
    void receiveHandshake(QString srcUserName, QString srcIp);

private:
    QString userName;

    QMap<QString, QStringList> m_availableUsers;
    QMap<QString, QString> m_connectedUsers;
    QStringList m_pendingUsers;

    QTimer m_discoveryBroadcastTimer;
    QUdpSocket m_discoveryBroadcastSocket;
    QUdpSocket m_discoveryListenerSocket;

    MessageTransceiver *m_messageTransceiver;


signals:
    void sendMessage(QString destIp, QByteArray data);
    void peerConnected(QString destination, QString destinationAddress);

public slots:
    void peerDisconnected(QString peerUserName);
    void receiveData(QString srcIp, QByteArray data);

private slots:
    void discoveryBroadcastTimeout();
    void discoveryBroadcastReceived();


};

#endif // USERMANAGER_H
