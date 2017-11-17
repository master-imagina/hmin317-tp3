QT       += core gui widgets

TARGET = plane
TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp \
    seasonmanager.cpp \
    particle.cpp \
    particleengine.cpp \
    utils.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    camera.h \
    seasonmanager.h \
    particle.h \
    particleengine.h \
    utils.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

CONFIG += c++11

# install
target.path = .
INSTALLS += target

DISTFILES +=
