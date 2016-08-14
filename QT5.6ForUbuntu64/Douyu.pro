#-------------------------------------------------
#
# Project created by QtCreator 2016-08-03T16:57:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Douyu
TEMPLATE = app


SOURCES += main.cpp\
        douyumainwindow.cpp \
    dy_encoder.cpp \
    dy_barrage_client.cpp \
    data_def.cpp \
    douyuthread.cpp \
    dy_show_thread.cpp

HEADERS  += douyumainwindow.h \
    dy_encoder.h \
    dy_barrage_client.h \
    data_def.h \
    douyuthread.h \
    dy_show_thread.h

FORMS    += douyumainwindow.ui
