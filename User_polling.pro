#-------------------------------------------------
#
# Project created by QtCreator 2017-03-29T10:25:04
#
#-------------------------------------------------

QT       += gui core
greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets
TARGET = User_polling
CONFIG   -= console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp \
    gui.cpp \
    xlslogger.cpp \
    jsonsaver.cpp

HEADERS += \
    gui.h \
    xlslogger.h \
    jsonsaver.h

