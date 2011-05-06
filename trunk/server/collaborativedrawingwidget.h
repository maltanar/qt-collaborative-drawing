#ifndef COLLABORATIVEDRAWINGWIDGET_H
#define COLLABORATIVEDRAWINGWIDGET_H

#include "basedrawingwidget.h"
#include <QtNetwork>

class CollaborativeDrawingWidget : public BaseDrawingWidget
{
    Q_OBJECT
public:
    explicit CollaborativeDrawingWidget(QWidget *parent = 0);

    // TODO this doesn't belong here
    QTcpSocket clientSocket;
    QTcpServer server;
    QTcpSocket *serverSocket;

    void startListening(int portNumber);

protected:
    void commitDrawing(QPicture drawingData);

signals:

public slots:
    void gotNewConnection();
    void dataArrived();

};

#endif // COLLABORATIVEDRAWINGWIDGET_H
