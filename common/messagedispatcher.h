#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <QObject>
#include <QMap>
#include <QList>

#include "usermanager.h"
#include "protocolhandler.h"

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
    void broadcastMessage(QByteArray msg);

public slots:
    void receiveMessage(QString origin, QByteArray msg);
    void sendMessage(QString destination, QByteArray msg);
    void connectionEstablished(QString destination, QString destinationAddress);


};

#endif // MESSAGEDISPATCHER_H
