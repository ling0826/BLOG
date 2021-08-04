#-------------------------------------------------
#
# Project created by QtCreator 2021-06-16T17:06:02
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
QT       += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UART
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    uart.cpp \
    form.cpp \
    network.cpp \
    uart_set.cpp \
    date_time.cpp \
    pthread/pthread_time.cpp \
    pthread/thread.cpp \
    cJSON/cJSON.c \
    Error_Data.c

HEADERS  += mainwindow.h \
    uart.h \
    form.h \
    network.h \
    uart_set.h \
    date_time.h \
    def.h \
    pthread/pthread_time.h \
    pthread/thread.h \
    cJSON/cJSON.h \
    Error_Data.h

FORMS    += mainwindow.ui \
    form.ui \
    uart_set.ui \
    date_time.ui

DISTFILES += \
    pthread.pri
