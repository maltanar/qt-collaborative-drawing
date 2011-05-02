#include <QtGui/QApplication>
#include "mainwindow.h"

#include <messagetransceiver.h>
#include <protocolhandler.h>
#include <collaborationclient.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MessageTransceiver m;
    ProtocolHandler p;
    CollaborationClient c;

    p.setMessageTransceiver(&m);
    c.setProtocolHandler(&p);
    p.addUserMapping(COLLABORATION_SERVER_NAME, "192.168.2.75");
    p.setUserName("Ozan");
    c.dummyFunction(COLLABORATION_SERVER_NAME);

    return a.exec();
}
