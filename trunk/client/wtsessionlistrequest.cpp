#include "wtsessionlistrequest.h"

WTSessionListRequest::WTSessionListRequest(QObject *parent) :
    WTMessage(parent)
{
    command = "SESSLREQ";
}
