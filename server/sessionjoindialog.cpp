#include "sessionjoindialog.h"
#include "ui_sessionjoindialog.h"

#include <QMessageBox>

SessionJoinDialog::SessionJoinDialog(CollaborationClient * client, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::SessionJoinDialog)
{
    ui->setupUi(this);
    m_client = client;

    connect(m_client, SIGNAL(foundCollaborationServer(QHostAddress)), this, SLOT(foundServer(QHostAddress)));
    connect(m_client, SIGNAL(loginResult(bool,QString)), this, SLOT(loginResult(bool,QString)));
    connect(m_client, SIGNAL(sessionListAvailable(QStringList)), this, SLOT(gotSessionList(QStringList)));
}

void SessionJoinDialog::foundServer(QHostAddress addr)
{
    ui->serverIP->setText(addr.toString());
}

SessionJoinDialog::~SessionJoinDialog()
{
    delete ui;
}

void SessionJoinDialog::on_refreshList_clicked()
{

}

void SessionJoinDialog::on_pushButton_clicked()
{
    ui->connectionStatus->setText("Connecting...");
    m_client->loginToServer(QHostAddress(ui->serverIP->text()), ui->userName->text());
}

void SessionJoinDialog::on_sessionList_doubleClicked(QModelIndex index)
{

}

void SessionJoinDialog::on_buttonBox_accepted()
{

}

void SessionJoinDialog::on_buttonBox_rejected()
{
    this->hide();
    this->deleteLater();
}

void SessionJoinDialog::loginResult(bool result, QString infoMsg)
{
    QMessageBox m;
    m.setText(infoMsg);
    m.exec();

    if(result)
        ui->connectionStatus->setText("Connected");
    else
        ui->connectionStatus->setText("Error!");
}

void SessionJoinDialog::gotSessionList(QStringList sessionList)
{
    ui->sessionList->clear();
    ui->sessionList->addItems(sessionList);
}
