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
    wtloginresponsemessage.cpp \
    wtrequestmessage.cpp \
    wtloginmessage.cpp \
    wtsessionlistrequest.cpp

HEADERS  += mainwindow.h \
    wtmessage.h \
    wtloginresponsemessage.h \
    wtrequestmessage.h \
    wtloginmessage.h \
    wtsessionlistrequest.h

FORMS    += mainwindow.ui
