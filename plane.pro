QT       += core gui widgets

TARGET = plane
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp \
    particle.cpp \
    particleemitter.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    particle.h \
    particleemitter.h

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    images.qrc

# install
target.path = .
INSTALLS += target

DISTFILES += \
    heightmap-1.png \
    heightmap-2.png \
    heightmap-3.png \
    heightmap-11.png
