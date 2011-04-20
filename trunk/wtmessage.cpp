#include "wtmessage.h"

WTMessage::WTMessage(QObject *parent) :
        QObject(parent), msgSize(16) //Constant header size
{
        version = "WTC1";
}

WTMessage::~WTMessage()
{

}

QByteArray WTMessage::serialize()
{
    QByteArray data(version.toAscii());
    data.append(command.toAscii());
    data.append(QByteArray::fromRawData((const char*)&msgSize, 4));
    return data;
}

void WTMessage::deserialize(QByteArray *data)
{
    //Assuming the header is always of the same size.
    memcpy(&msgSize,data->right(data->length()-12).left(4).constData(), 4);
}
