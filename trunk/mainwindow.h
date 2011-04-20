#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QtNetwork>

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

public slots:
    void yoksunsenaslinda();
    void dataArrived();
    void gotNewConnection();
    void startListening();
};

#endif // MAINWINDOW_H
