#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <QObject>
#include <QMap>
#include <QList>

#include "usermanager.h"
#include "protocolhandler.h"

// we define a special sender identity for broadcast messages sent to all protocol handlers
// (e.g when a user is detected to be disconnected)
#define PROTOCOLHANDLER_BROADCAST   QString("$protocol_handler_broadcast$")

class ProtocolHandler;

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit MessageDispatcher(QObject *parent = 0);
    void subscribe(ProtocolHandler *protocolHandler, QStringList prefixes);
    void unsubscribe(ProtocolHandler *protocolHandler);

    void setMessageTransceiver(MessageTransceiver *messageTransceiver);
    MessageTransceiver *getMessageTransceiver();

    void setUserManager(UserManager *newUserManager);
    UserManager* getUserManager();

private:
    QMap<QString, ProtocolHandler *> m_subscriptionList;
    QMap<QString, QByteArray> m_bufferedMessages;
    QList<ProtocolHandler *> m_protocolHandlers;
    MessageTransceiver *m_messageTransceiver;
    UserManager *m_userManager;

signals:
    void signalMessageBroadcast(QString origin, QByteArray msg);

public slots:
    void receiveMessage(QString origin, QByteArray msg);
    void broadcastRequestSlot(QString origin, QByteArray msg);
    void sendMessage(QString destination, QByteArray msg);
    void connectionEstablished(QString destination, QString destinationAddress);


};

#endif // MESSAGEDISPATCHER_H
