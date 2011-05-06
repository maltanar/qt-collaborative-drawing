#include "wtlogoutrequest.h"

WTLogoutRequest::WTLogoutRequest(QObject *parent) :
    WTMessage(parent)
{
    command = "LOGOUTRQ";
}
