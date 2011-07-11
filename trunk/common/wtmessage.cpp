#include "wtmessage.h"

WTMessage::WTMessage() :
        msgSize(HEADER_SIZE) //Constant header size + username
{
    version = "WTC1";
    m_serializerBuffer.setBuffer(&m_serializedData);
    m_serializerBuffer.open(QIODevice::ReadWrite);
    m_serializer.setDevice(&m_serializerBuffer);
}

WTMessage::~WTMessage()
{
    m_serializerBuffer.close();
}

QByteArray WTMessage::serialize()
{
    // serialize the message
    // first, clear any existing serialized data in the buffer
    m_serializedData.clear();
    m_serializerBuffer.close();
    m_serializerBuffer.setBuffer(&m_serializedData);
    m_serializerBuffer.open(QIODevice::ReadWrite);
    m_serializer.unsetDevice();
    m_serializer.setDevice(&m_serializerBuffer);

    // use Qt's QDataStream << operator to serialize the fields
    m_serializer << version << command << msgSize << srcUsername << destUsername;

    return m_serializedData;
}

void WTMessage::deserialize(QByteArray data)
{
    // deserialize using Qt QDataStream >> operator
    m_serializedData = data;
    m_serializerBuffer.close();
    m_serializerBuffer.setBuffer(&m_serializedData);
    m_serializerBuffer.open(QIODevice::ReadWrite);
    m_serializer.unsetDevice();
    m_serializer.setDevice(&m_serializerBuffer);

    m_serializer >> version >> command >> msgSize >> srcUsername >> destUsername;
}


void WTMessage::setSrcUsername(QString username)
{
    this->srcUsername = username;
}

QString WTMessage::getSrcUsername()
{
    return this->srcUsername;
}

void WTMessage::setDestUsername(QString username)
{
    this->destUsername = username;
}

QString WTMessage::getDestUsername()
{
    return this->destUsername;
}

void WTMessage::setCommand(QString command)
{
    this->command = command;
}

QString WTMessage::getCommand()
{
    return this->command;
}

void WTMessage::setVersion(QString version)
{
    this->version = version;
}

QString WTMessage::getVersion()
{
    return this->version;
}
