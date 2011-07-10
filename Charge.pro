#-------------------------------------------------
#
# Project created by QtCreator 2011-07-09T23:43:22
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Charge
TEMPLATE = app

LIBS = -lBox2D

SOURCES +=\
    src/staticobstacle.cpp \
    src/player.cpp \
    src/field.cpp \
    src/dynamicobstacle.cpp \
    src/circularfield.cpp \
    src/actor.cpp \
    src/mainwindow.cpp \
    src/maincanvas.cpp \
    src/main.cpp \
    src/settingsdialog.cpp \
    src/chargeloader.cpp

HEADERS  += \
    src/staticobstacle.h \
    src/player.h \
    src/field.h \
    src/dynamicobstacle.h \
    src/circularfield.h \
    src/actor.h \
    src/mainwindow.h \
    src/maincanvas.h \
    src/settingsdialog.h \
    src/chargeloader.h
