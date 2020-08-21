#-------------------------------------------------
#
# Project created by QtCreator 2018-02-20T07:16:38
#
#-------------------------------------------------

QT       += core gui
QT       += xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MS03
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    dcmotor.cpp \
    mobileplatform.cpp \
    linesensor.cpp \
    controlsystem.cpp \
    colorsensor.cpp \
    abstractsensor.cpp \
    encoder.cpp \
    ultrasonicsensor.cpp \
    odometrie.cpp \
    qcustomplot.cpp \
    xmlprasen.cpp \
    sorter.cpp

HEADERS  += mainwindow.h \
    dcmotor.h \
    mobileplatform.h \
    linesensor.h \
    controlsystem.h \
    colorsensor.h \
    abstractsensor.h \
    encoder.h \
    ultrasonicsensor.h \
    odometrie.h \
    qcustomplot.h \
    sorter.h \
    xmlparsen.h


FORMS    += mainwindow.ui
LIBS += -lwiringPi -lpthread
