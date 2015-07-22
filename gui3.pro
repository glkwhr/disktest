#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T11:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = gui3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintedwidget.cpp \
    iozone.c \
    libasync.c \
    libbif.c \
    iozone_event.cpp \
    iozone_paramwidget.cpp \
    iozone_widget.cpp \
    postmark_paramwidget.cpp \
    postmark_widget.cpp \
    postmark_event.cpp \
    postmark_thread.cpp \
    postmark-1.53.c \
    iozone_thread.cpp \
    qcustomplot.cpp \
    iozone_chart.cpp

HEADERS  += mainwindow.h \
    paintedwidget.h \
    iozone_event.h \
    iozone_paramwidget.h \
    iozone_widget.h \
    postmark_paramwidget.h \
    postmark_widget.h \
    postmark_event.h \
    postmark_thread.h \
    iozone_thread.h \
    qcustomplot.h \
    iozone_chart.h

DEFINES = unix HAVE_ANSIC_C ASYNC_IO NAME='"linux-AMD64"' __AMD64__ SHARED_MEM linux _LARGEFILE64_SOURCE HAVE_PREAD

LIBS += -lrt -lpthread

RESOURCES += \
    res.qrc
