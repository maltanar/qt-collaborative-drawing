#ifndef WTMESSAGE_H
#define WTMESSAGE_H

#include <QObject>

class WTMessage : public QObject
{
    Q_OBJECT

public:
    explicit WTMessage(QObject *parent = 0);
    ~WTMessage();
    virtual QByteArray serialize();
    //TODO Decide on the type of the argument whether it should be constant and reference.
    virtual void deserialize(QByteArray *data);

protected:
    QString version;
    QString command;
    int msgSize;


};

#endif // WTMESSAGE_H
