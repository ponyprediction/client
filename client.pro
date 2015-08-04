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
        ui/main-window.cpp \
    ui/connection-form.cpp \
    ui/logs-form.cpp \
    core/util.cpp \
    core/client.cpp \
    core/job.cpp \
    core/brain.cpp \
    core/problem.cpp \
    core/neuron.cpp \
    core/neuron-blueprint.cpp \
    ui/control-form.cpp \
    ui/local-form.cpp \
    core/connection.cpp

HEADERS  += ui/main-window.hpp \
    ui/connection-form.hpp \
    ui/logs-form.hpp \
    core/util.hpp \
    core/client.hpp \
    core/job.hpp \
    core/brain.hpp \
    core/problem.hpp \
    core/neuron.hpp \
    core/neuron-blueprint.hpp \
    ui/control-form.hpp \
    ui/local-form.hpp \
    core/connection.hpp

FORMS    += ui/main-window.ui \
    ui/connection-form.ui \
    ui/logs-form.ui \
    ui/control-form.ui \
    ui/local-form.ui

DISTFILES += \
    client.conf
