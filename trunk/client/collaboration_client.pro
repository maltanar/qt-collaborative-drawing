#-------------------------------------------------
#
# Project created by QtCreator 2011-04-16T14:28:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = watchntouch_collaboration
TEMPLATE = app

COMMON_SOURCES +=   ../common/wtmessage.cpp \
                     ../common/wtloginmessage.cpp \
                     ../common/wtsessionlistrequest.cpp \
                     ../common/messagetransceiver.cpp \
                     ../common/wtlogoutrequest.cpp \
                     ../common/wtpicturerequest.cpp \
                     ../common/wtpictureresponse.cpp \
                     ../common/wtsessionjoinrequest.cpp \
                     ../common/wtsessionjoinresponse.cpp \
                     ../common/wtsessionleaverequest.cpp \
                     ../common/wtsessionleaveresponse.cpp \
                     ../common/wtsessionlistresponse.cpp \
                     ../common/wtsessionmemberupdate.cpp \
                     ../common/wtupdatedrawing.cpp \
                     ../common/wtwritepermissionrequest.cpp \
                     ../common/wtwritepermissionstatus.cpp \
                     ../common/wtloginresponse.cpp \
                     ../common/protocolhandler.cpp \
                     ../common/wtpeerhandshake.cpp \
                     ../common/collaborationsession.cpp 

COMMON_HEADERS +=    ../common/wtmessage.h \
                     ../common/wtloginmessage.h \
                     ../common/wtsessionlistrequest.h \
                     ../common/messagetransceiver.h \
                     ../common/wtlogoutrequest.h \
                     ../common/wtpicturerequest.h \
                     ../common/wtpictureresponse.h \
                     ../common/wtsessionjoinrequest.h \
                     ../common/wtsessionjoinresponse.h \
                     ../common/wtsessionleaverequest.h \
                     ../common/wtsessionleaveresponse.h \
                     ../common/wtsessionlistresponse.h \
                     ../common/wtsessionmemberupdate.h \
                     ../common/wtupdatedrawing.h \
                     ../common/wtwritepermissionrequest.h \
                     ../common/wtwritepermissionstatus.h \
                     ../common/wtloginresponse.h \
                     ../common/protocolhandler.h \
                     ../common/wtpeerhandshake.h \
                     ../common/collaborationsession.h

INCLUDEPATH += . \
    ../common

SOURCES += main.cpp\
        mainwindow.cpp \
	collaborationclient.cpp \
	basedrawingwidget.cpp \
	drawingdata.cpp \
	collaborativedrawingwidget.cpp \
	drawingaction.cpp \
	sessionjoindialog.cpp \
	$$COMMON_SOURCES

HEADERS  += mainwindow.h \
    collaborationclient.h \
    appglobals.h \
    drawingdata.h \
    drawingaction.h \
    basedrawingwidget.h \
    collaborativedrawingwidget.h \
    sessionjoindialog.h \
    $$COMMON_HEADERS

FORMS    += mainwindow.ui \
    sessionjoindialog.ui
