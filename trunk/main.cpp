#include <QtGui/QApplication>
#include "mainwindow.h"
#include "messagetransceiver.h"
#include "protocolhandler.h"
#include "collaborationserver.h"
#include "collaborationclient.h"

#include <messagetransceiver.h>
#include <protocolhandler.h>
#include <collaborationclient.h>

#include "collaborationsession.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    /*MessageTransceiver m;
    ProtocolHandler p;
    CollaborationServer cs;

    p.setMessageTransceiver(&m);
    cs.setProtocolHandler(&p);*/

    CollaborationSession sess;
    sess.setSessionName("naber");
    sess.setSessionPassword("lol");
    QPicture hede;
    QPainter hodo;
    hodo.begin(&hede);
    hodo.drawEllipse(100,100, 50,50);
    hodo.end();
    sess.addDrawingStep(hede);

    hodo.begin(&hede);
    hodo.drawEllipse(100,100, 75,75);
    hodo.end();
    sess.addDrawingStep(hede);

    QPicture zit = sess.getSessionDrawingState();
    QPixmap lulz(200,200);
    lulz.fill(Qt::white);
    hodo.begin(&lulz);
    hodo.drawPicture(0,0,zit);
    hodo.end();

    lulz.save("naber.png");


    MessageTransceiver m;
    ProtocolHandler p;
    CollaborationClient c;

    p.setMessageTransceiver(&m);
    c.setProtocolHandler(&p);
    p.addUserMapping(COLLABORATION_SERVER_NAME, "192.168.2.75");
    p.setUserName("Ozan");
    c.dummyFunction(COLLABORATION_SERVER_NAME);

    m.run();


    return a.exec();
}
