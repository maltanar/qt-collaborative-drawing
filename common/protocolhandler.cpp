#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent):
    QObject(parent)
{
}

void ProtocolHandler::receiveData(QString origin, QByteArray data)
{

}


void ProtocolHandler::sendMessage(QString toUsername, QByteArray data)
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
}

void ProtocolHandler::setMessageTransceiver(MessageTransceiver * newMesssageTransceiver)
{
    if(m_messageTransceiver) {
        // disconnect all signals and slots from previous MessageTransceiver
        disconnect(this);
        disconnect(m_messageTransceiver);
    }
    // connect signals and slots
    connect(this, SIGNAL(sendMessage(QString,QByteArray)), newMesssageTransceiver, SLOT(sendMessage(QString,QByteArray)));
    //connect(newMesssageTransceiver, SIGNAL(gotNewData(QString,QByteArray)), this, SLOT(receiveMessage(QString,QByteArray)));
    connect(newMesssageTransceiver, SIGNAL(clientDisconnected(QString)), this, SLOT(clientDisconnected(QString)));
}

MessageTransceiver* ProtocolHandler::getMessageTransceiver()
{
    return m_messageTransceiver;
}
