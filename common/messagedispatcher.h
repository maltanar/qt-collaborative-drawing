#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <QObject>
#include <protocolhandler.h>
#include <QMap>

class MessageDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit MessageDispatcher(QObject *parent = 0);
    void subscribe(ProtocolHandler *protocolHandler, QStringList prefixes);
    void unsubscribe(ProtocolHandler *protocolHandler);

    void setMessageTransceiver(MessageTransceiver *messageTransceiver);
    MessageTransceiver *getMessageTransceiver();

private:
    QMap<QString, ProtocolHandler *> m_protocolHandlers;
    MessageTransceiver *m_messageTransceiver;

signals:

public slots:
    void receiveMessage(QString origin, QByteArray msg);


};

#endif // MESSAGEDISPATCHER_H
