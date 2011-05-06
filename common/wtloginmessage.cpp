#include "wtloginmessage.h"

WTLoginMessage::WTLoginMessage(QObject *parent) :
    WTMessage(parent)
{
    command = "LOGINREQ";
}
