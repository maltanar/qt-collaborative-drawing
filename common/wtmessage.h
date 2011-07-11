#ifndef WTMESSAGE_H
#define WTMESSAGE_H

#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
#include <QBuffer>

#define HEADER_SIZE 32

class WTMessage
{

public:
    explicit WTMessage();
    ~WTMessage();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray data);
    void setSrcUsername(QString username);
    QString getSrcUsername();
    void setDestUsername(QString username);
    QString getDestUsername();
    void setCommand(QString command);
    QString getCommand();
    void setVersion(QString version);
    QString getVersion();

protected:
    QString version;
    QString command;
    int msgSize;
    QString srcUsername;
    QString destUsername;
    QByteArray m_serializedData;
    QBuffer m_serializerBuffer;
    QDataStream m_serializer;

};

#endif // WTMESSAGE_H
