#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QtNetwork>

#include "messagetransceiver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPushButton *btn;
    QLineEdit *tb;
    QUdpSocket server;
    QUdpSocket client;

private:
    Ui::MainWindow *ui;
    MessageTransceiver mt;

public slots:
    void dataArrived();
    void gotNewConnection();
    void startListening();

    void gotNewData(QString origin, QByteArray data);

signals:
    void connectToHost(QString addr);
    void sendData(QString addr, QByteArray data);

private slots:
    void on_sendData_clicked();
    void on_connectToHost_clicked();
};

#endif // MAINWINDOW_H
