#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTextEdit>
#include <wtloginmessage.h>
#include <wtloginresponse.h>
#include <wtlogoutrequest.h>
#include <wtpicturerequest.h>
#include <wtpictureresponse.h>
#include <wtsessionjoinrequest.h>
#include <wtsessionjoinresponse.h>
#include <wtsessionleaverequest.h>
#include <wtsessionleaveresponse.h>
#include <wtsessionlistrequest.h>
#include <wtsessionlistresponse.h>
#include <wtsessionmemberupdate.h>
#include <wtupdatedrawing.h>
#include <wtwritepermissionrequest.h>
#include <wtwritepermissionstatus.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&mt, SIGNAL(newData(QString,QByteArray)), this, SLOT(gotNewData(QString,QByteArray))) ;
    connect(this, SIGNAL(connectToHost(QString)), &mt, SLOT(connectTo(QString)));
    connect(this, SIGNAL(sendData(QString,QByteArray)), &mt, SLOT(sendMessage(QString,QByteArray)));

    mt.start();

    WTLoginResponse msg;
    msg.setUsername("ozan");
    msg.setInfomsg("merhaba");



    QByteArray *packet1 =  new QByteArray(msg.serialize());
    WTLoginResponse msgTest;
    msgTest.deserialize(packet1);
    qWarning() << msg.getUsername() << " : " << msg.getInfomsg();


    //TODO Used for testing, remove later.
    /*QPushButton *quit = new QPushButton(tr("Quit"), this);
    quit->setGeometry(10,130,50,50);
    btn = new QPushButton(tr("merhaaaaba"), this);
    btn->setGeometry(10,70,50,50);
    QPushButton *listen = new QPushButton(tr("Listen"), this);
    tb = new QLineEdit(this);
    tb->setGeometry(70,10,100,20);
    listen->setGeometry(10,10,50,50);

    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(listen, SIGNAL(clicked()), this, SLOT(startListening()));
    connect(btn, SIGNAL(clicked()), this, SLOT(yoksunsenaslinda()));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::yoksunsenaslinda()
{
    QByteArray datagram;
    WTLoginMessage *msg = new WTLoginMessage();
    msg->setUsername(tb->text());
    datagram = msg->serialize();
    qWarning("%s\n",datagram.constData());
    server.writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 12345);
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
