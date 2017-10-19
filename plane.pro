QT       += core gui widgets

TARGET = plane
TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp \
    seasonmanager.cpp \
    particles.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    camera.h \
    seasonmanager.h \
    particles.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

CONFIG += c++11

# install
target.path = .
INSTALLS += target
