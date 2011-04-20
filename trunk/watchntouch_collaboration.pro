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
    wtloginresponsemessage.cpp \
    wtsessionlistrequest.cpp

HEADERS  += mainwindow.h \
    wtmessage.h \
    wtloginmessage.h \
    wtloginresponsemessage.h \
    wtsessionlistrequest.h

FORMS    += mainwindow.ui
