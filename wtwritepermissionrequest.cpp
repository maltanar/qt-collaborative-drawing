#include "wtwritepermissionrequest.h"

WTWritePermissionRequest::WTWritePermissionRequest(QObject *parent) :
    WTMessage(parent)
{
    command = "WRTPRMRQ";
}
