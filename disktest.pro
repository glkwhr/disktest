#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T11:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = diskTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
	iozone/iozone.c \
	iozone/libasync.c \
	iozone/libbif.c \
	iozone/iozone_event.cpp \
	iozone/iozone_paramwidget.cpp \
	iozone/iozone_widget.cpp \
	postmark/postmark_paramwidget.cpp \
	postmark/postmark_widget.cpp \
	postmark/postmark_event.cpp \
	postmark/postmark_thread.cpp \
	postmark/postmark-1.53.c \
	iozone/iozone_thread.cpp \
	qcustomplot/qcustomplot.cpp \
	iozone/iozone_chart.cpp \
	postmark/postmark_chart.cpp \
	iozone/iozone_rate_model.cpp


HEADERS  += mainwindow.h \
	iozone/iozone_event.h \
	iozone/iozone_paramwidget.h \
	iozone/iozone_widget.h \
	postmark/postmark_paramwidget.h \
	postmark/postmark_widget.h \
	postmark/postmark_event.h \
	postmark/postmark_thread.h \
	iozone/iozone_thread.h \
	qcustomplot/qcustomplot.h \
	iozone/iozone_chart.h \
	postmark/postmark_chart.h \
	iozone/iozone_rate_model.h


DEFINES = unix HAVE_ANSIC_C ASYNC_IO NAME='"linux-AMD64"' __AMD64__ SHARED_MEM linux _LARGEFILE64_SOURCE HAVE_PREAD

LIBS += -lrt -lpthread

RESOURCES += \
    res.qrc
