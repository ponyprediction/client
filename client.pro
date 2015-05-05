#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T11:15:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
        main-window.cpp \
    ui/connection-form.cpp \
    ui/logs-form.cpp \
    core/util.cpp \
    core/client.cpp

HEADERS  += main-window.hpp \
    ui/connection-form.hpp \
    ui/logs-form.hpp \
    core/util.hpp \
    core/client.hpp

FORMS    += main-window.ui \
    ui/connection-form.ui \
    ui/logs-form.ui

DISTFILES += \
    conf.xml
