#ifndef WTMESSAGE_H
#define WTMESSAGE_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QStringList>

#define HEADER_SIZE 32

class WTMessage : public QObject
{
    Q_OBJECT

public:
    explicit WTMessage(QObject *parent = 0);
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


};

#endif // WTMESSAGE_H
