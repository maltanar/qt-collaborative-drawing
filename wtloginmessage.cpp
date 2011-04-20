#include "wtloginmessage.h"

WTLoginMessage::WTLoginMessage(QObject *parent) :
    WTRequestMessage(parent)
{
    command = "LOGINREQ";
}
