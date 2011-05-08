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
    void drawingCommited(QString sessionName, QPicture drawingData);

public slots:
    void gotNewConnection();
    void dataArrived();
    void gotDrawingData(QString sessionName, QByteArray picData);

};

#endif // COLLABORATIVEDRAWINGWIDGET_H
