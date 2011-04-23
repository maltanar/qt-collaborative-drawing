#ifndef WTMESSAGE_H
#define WTMESSAGE_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QStringList>

class WTMessage : public QObject
{
    Q_OBJECT

public:
    explicit WTMessage(QObject *parent = 0);
    ~WTMessage();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray *data);
    void setUsername(QString username);
    QString getUsername();

protected:
    QString version;
    QString command;
    int msgSize;
    QString username;


};

#endif // WTMESSAGE_H
