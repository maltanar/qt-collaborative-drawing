#include "collaborativedrawingwidget.h"

CollaborativeDrawingWidget::CollaborativeDrawingWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{
    //serverSocket = NULL;
    //connect(&server, SIGNAL(newConnection()), this, SLOT(gotNewConnection()));
}

void CollaborativeDrawingWidget::commitDrawing(QPicture drawingPictureData)
{
    // TODO the actual data sending shouldn't take place here
    // TODO implement a proper protocol with error checking!

    qWarning() << "Collab commitDrawing size" << drawingPictureData.size();

    BaseDrawingWidget::commitDrawing(drawingPictureData);
    //TODO Session name will not be static!!!!
    emit drawingCommited("test",drawingPictureData);


    /*QByteArray picdata(drawingPictureData.data(), drawingPictureData.size());
    int dsize = drawingPictureData.size();
    QByteArray datalen = QByteArray::fromRawData((const char*)&dsize, 4);;

    qWarning() << "written header # " << clientSocket.write(QString("DRAW").toAscii());
    qWarning() << "written length # " << clientSocket.write(datalen) << datalen;
    qWarning() << "written bytes #" << clientSocket.write(picdata);*/


}

void CollaborativeDrawingWidget::startListening(int portNumber)
{
    qWarning() << "server starting to listen";
    server.listen(QHostAddress::Any, portNumber);
}


void CollaborativeDrawingWidget::gotNewConnection()
{
    qWarning() << "server got new connection!";

    serverSocket = server.nextPendingConnection();
    connect(serverSocket, SIGNAL(readyRead()), this, SLOT(dataArrived()));
}


void CollaborativeDrawingWidget::dataArrived()
{
    static int sizeOfPackage = -1;
    static QByteArray receivedData;

    qWarning() << "new data arrived to server!";
    QByteArray newdata = serverSocket->readAll();
    qWarning() << "size of arrived data" << newdata.size();

    if(newdata.startsWith(QString("DRAW").toAscii())) {
        qWarning() << "raw data" << newdata.right(newdata.length() - 4).left(4).toHex();
        memcpy(&sizeOfPackage, newdata.right(newdata.length() - 4).left(4).constData(), 4);
        qWarning() << "got sizeofpackage " << sizeOfPackage;
        receivedData = newdata.right(newdata.length() - 8);
    } else {
        receivedData.append(newdata);
    }

    qWarning() << "sizeofpackage " << sizeOfPackage << "total recv data size" << receivedData.size();

    if(sizeOfPackage == receivedData.size()) {
        // done receiving data for this drawing step
        QPicture pic;
        pic.setData(receivedData.constData(), receivedData.size());
        BaseDrawingWidget::commitDrawing(pic);
        sizeOfPackage = -1;
        receivedData = QByteArray();
    }
}


void CollaborativeDrawingWidget::gotDrawingData(QString sessionName, QByteArray picData)
{
    QPicture pic;
    qWarning() << "Resim geldiii of size : " << picData.size();
    pic.setData(picData.constData(), picData.size());
    BaseDrawingWidget::commitDrawing(pic);
}
