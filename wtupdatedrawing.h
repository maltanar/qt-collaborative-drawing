#ifndef WTUPDATEDRAWING_H
#define WTUPDATEDRAWING_H

#include <wtmessage.h>

class WTUpdateDrawing : public WTMessage
{
    Q_OBJECT
public:
    explicit WTUpdateDrawing(QObject *parent = 0);

    QByteArray serialize();
    void deserialize(QByteArray *data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QByteArray getPicData();
    void setPicData(QByteArray picData);

private:
    QString sessionName;
    QByteArray picData;
};

#endif // WTUPDATEDRAWING_H
