#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include "collaborationclient.h"
#include "sessionjoindialog.h"
#include <messagedispatcher.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CollaborationClient *client;
    SharedCanvasProtocolHandler *ph;
    MessageTransceiver *mt;
    MessageDispatcher *mp;


private slots:
    void on_actionLeave_triggered();
    void on_actionConnect_triggered();
    void on_actionPenWidthDec_triggered();
    void on_actionPenWidthInc_triggered();
    void on_actionBluePen_triggered();
    void on_actionBlackPen_triggered();
    void on_actionRedPen_triggered();
    void on_actionEraser_triggered();
    void sessionJoinResult(QString sessionName, QChar result, QMap<QString, qint32> users);
    void drawingCommitted(QString sessionName, QPicture pictureData);
    void on_actionStraightLine_triggered();
    void on_actionRectangle_triggered();
    void on_actionFreehand_triggered();
    void on_actionClear_triggered();
};

#endif // MAINWINDOW_H
