#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent)
{
}

void ProtocolHandler::receiveMessage(QString origin, QByteArray data)
{

}

bool ProtocolHandler::deliverMessage(WTMessage * msg)
{
    return true;
}
