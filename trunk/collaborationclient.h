#ifndef COLLABORATIONCLIENT_H
#define COLLABORATIONCLIENT_H

#include <QObject>

class CollaborationClient : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationClient(QObject *parent = 0);

signals:

public slots:

};

#endif // COLLABORATIONCLIENT_H
