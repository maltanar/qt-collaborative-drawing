#-------------------------------------------------
#
# Project created by QtCreator 2011-04-16T14:28:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = watchntouch_collaboration
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wtmessage.cpp \
    wtloginmessage.cpp \
    wtsessionlistrequest.cpp \
    messagetransceiver.cpp \
    wtlogoutrequest.cpp \
    wtpicturerequest.cpp \
    wtpictureresponse.cpp \
    wtsessionjoinrequest.cpp \
    wtsessionjoinresponse.cpp \
    wtsessionleaverequest.cpp \
    wtsessionleaveresponse.cpp \
    wtsessionlistresponse.cpp \
    wtsessionmemberupdate.cpp \
    wtupdatedrawing.cpp \
    wtwritepermissionrequest.cpp \
    wtwritepermissionstatus.cpp \
    wtloginresponse.cpp \
    protocolhandler.cpp \
    wtpeerhandshake.cpp \
    collaborationserver.cpp \
    collaborationclient.cpp

HEADERS  += mainwindow.h \
    wtmessage.h \
    wtloginmessage.h \
    wtsessionlistrequest.h \
    messagetransceiver.h \
    wtlogoutrequest.h \
    wtpicturerequest.h \
    wtpictureresponse.h \
    wtsessionjoinrequest.h \
    wtsessionjoinresponse.h \
    wtsessionleaverequest.h \
    wtsessionleaveresponse.h \
    wtsessionlistresponse.h \
    wtsessionmemberupdate.h \
    wtupdatedrawing.h \
    wtwritepermissionrequest.h \
    wtwritepermissionstatus.h \
    wtloginresponse.h \
    protocolhandler.h \
    wtpeerhandshake.h \
    collaborationserver.h \
    collaborationclient.h

FORMS    += mainwindow.ui
