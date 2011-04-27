#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&mt, SIGNAL(gotNewData(QString,QByteArray)), this, SLOT(gotNewData(QString,QByteArray))) ;
    connect(this, SIGNAL(connectToHost(QString)), &mt, SLOT(connectTo(QString)));
    connect(this, SIGNAL(sendData(QString,QByteArray)), &mt, SLOT(sendMessage(QString,QByteArray)));

    mt.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::startListening()
{
    client.bind(12345, QUdpSocket::ShareAddress);
    connect(&client, SIGNAL(readyRead()),
            this, SLOT(dataArrived()));

}

void MainWindow::gotNewConnection()
{
    qWarning() << "new connection" << endl;
}

void MainWindow::dataArrived()
{
    QByteArray *datagram = new QByteArray();
    while(client.hasPendingDatagrams())
    {

        datagram->resize(client.pendingDatagramSize());
        client.readDatagram(datagram->data(), datagram->size());

    }
}

void MainWindow::on_connectToHost_clicked()
{
    QString host = ui->hostAddress->text();
    emit connectToHost(host);
}

void MainWindow::on_sendData_clicked()
{
    QString host = ui->hostAddress->text();
    QByteArray data = ui->inputData->text().toAscii();

    emit sendData(host, data);
}

void MainWindow::gotNewData(QString origin, QByteArray data)
{
    ui->receivedData->setPlainText(ui->receivedData->toPlainText() + "\n" + origin + " length: " + QString::number(data.length()));
}
