#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent):
    QObject(parent)
{
    m_messageTransceiver = NULL;
    m_messageDispatcher = NULL;
}

void ProtocolHandler::receiveData(QString origin, QByteArray data)
{

}


void ProtocolHandler::setMessageDispatcher(MessageDispatcher * messageDispatcher)
{
    // This protocol handler has already been subscribed to the message dispatcher
    // - unsubscribe it!
    if (m_messageDispatcher)
    {
        m_messageDispatcher->unsubscribe(this);
    }

    m_messageDispatcher = messageDispatcher;

    connect(this, SIGNAL(sendMessage(QString, QByteArray)), m_messageDispatcher, SLOT(sendMessage(QString,QByteArray)));
}

void ProtocolHandler::setMessageTransceiver(MessageTransceiver * newMesssageTransceiver)
{
    if(m_messageTransceiver) {
        // disconnect all signals and slots from previous MessageTransceiver
        disconnect(this);
        disconnect(m_messageTransceiver);
    }
    // connect signals and slots

    //connect(this, SIGNAL(sendMessage(QString,QByteArray)), newMesssageTransceiver, SLOT(sendMessage(QString,QByteArray)));

    connect(newMesssageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
}

MessageTransceiver* ProtocolHandler::getMessageTransceiver()
{
    return m_messageTransceiver;
}
