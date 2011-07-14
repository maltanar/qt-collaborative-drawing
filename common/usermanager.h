#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QBuffer>
#include <QDebug>
#include "messagetransceiver.h"

#define USER_HANDSHAKE_SIGNATURE "EKTHS"

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

protected:
    void sendHandshake(QString destIp);
    void receiveHandshake(QString srcUserName, QString srcIp);

private:
    QString userName;

    QMap<QString, QStringList> m_availableUsers;
    QMap<QString, QString> m_connectedUsers;
    QStringList m_pendingUsers;

    MessageTransceiver *m_messageTransceiver;


signals:
    void sendMessage(QString destIp, QByteArray data);
    void peerConnected(QString destination, QString destinationAddress);

public slots:
    void peerDisconnected(QString peerUserName);
    void receiveData(QString srcIp, QByteArray data);


};

#endif // USERMANAGER_H
