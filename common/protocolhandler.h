#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <messagetransceiver.h>
#include "messagedispatcher.h"

// we define a special sender identity for broadcast messages sent to all protocol handlers
// (e.g when a user is detected to be disconnected)
#define PROTOCOLHANDLER_BROADCAST   QString("$protocol_handler_broadcast$")

class MessageDispatcher;

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = 0);

    virtual void setMessageDispatcher(MessageDispatcher * messageDispatcher);

    void setMessageTransceiver(MessageTransceiver * newMesssageTransceiver);
    MessageTransceiver* getMessageTransceiver();

protected:
    MessageDispatcher * m_messageDispatcher;
    MessageTransceiver * m_messageTransceiver;

signals:
    void sendMessage(QString toUsername, QByteArray data);


public slots:
    virtual void receiveData(QString origin, QByteArray data);
    virtual void receiveBroadcast(QByteArray data);
};

#endif // PROTOCOLHANDLER_H
