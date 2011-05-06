#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingdata.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DrawingData *drawingData = new DrawingData(this);
    ui->graphicsView->setDrawingData(drawingData);
    connect(ui->actionUndo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(redo()));

    mt = new MessageTransceiver();
    ph = new ProtocolHandler();
    ph->setMessageTransceiver(mt);
    client = new CollaborationClient();
    client->setProtocolHandler(ph);

    connect(client, SIGNAL(sessionJoinResult(QString,QChar,QHash<QString,long>)), this, SLOT(sessionJoinResult(QString,QChar,QHash<QString,long>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRedPen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::red);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionBlackPen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::black);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionBluePen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::blue);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionPenWidthInc_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setWidth(current.width()+1);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionPenWidthDec_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setWidth(current.width()-1);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionFreehand_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionRectangle_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_RECTANGLE);
}

void MainWindow::on_actionStraightLine_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_STRAIGHTLINE);
}

void MainWindow::on_actionEllipse_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_ELLIPSE);
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PNG","", "Portable Network Graphics (*.png)");
    if(fileName != "")
        ui->graphicsView->getDrawingData()->saveImage(fileName);
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Save SVG","", "Portable Network Graphics (*.png)");
    if(fileName != "")
        ui->graphicsView->getDrawingData()->loadImage(fileName);
}

void MainWindow::on_actionListen_triggered()
{
    QString portstr = QInputDialog::getText(this, "Port number",
                                              "Port number to listen on:", QLineEdit::Normal,
                                              "1337");

    ui->graphicsView->startListening(portstr.toInt());
}

void MainWindow::on_actionConnect_triggered()
{
    SessionJoinDialog * dlg = new SessionJoinDialog(client, this);
    dlg->show();
}

void MainWindow::on_actionEraser_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_ERASER);
}

void MainWindow::sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users)
{
    qWarning() << "Session join result: " << sessionName << " : " << result << " : " << users.count();
}
