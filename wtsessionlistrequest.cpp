#include "wtsessionlistrequest.h"

WTSessionListRequest::WTSessionListRequest(QObject *parent) :
    WTRequestMessage(parent)
{
    command = "SESSLREQ";
}
