#-------------------------------------------------
#
# Project created by QtCreator 2011-07-09T23:43:22
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Charge
TEMPLATE = app

LIBS += -lBox2D
DEFINES += GLEW_STATIC

QMAKE_POST_LINK = cp -r \"$$PWD/data\" \"$$OUT_PWD\";
macx:debug:QMAKE_POST_LINK += cp -r \"$$PWD/data\" \"$$OUT_PWD/$${TARGET}.app/Contents/MacOS\";

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
    src/chargeloader.cpp \
    src/objectloader.cpp \
    src/glew.cpp \
    src/model.cpp \
    src/light.cpp

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
    src/chargeloader.h \
    src/objectloader.h \
    src/glew.h \
    src/wglew.h \
    src/glxew.h \
    src/model.h \
    src/light.h

OTHER_FILES += \
    data/shaders/fragment/player.glsl \
    data/shaders/fragment/ambient.glsl \
    data/shaders/fragment/light.glsl \
    data/shaders/vertex/light.glsl \
    data/shaders/vertex/default.glsl \
    data/shaders/fragment/default.glsl \
    data/shaders/vertex/ambient.glsl \
    data/shaders/vertex/ambient.glsl
