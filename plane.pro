QT       += core gui widgets

TARGET = plane
TEMPLATE = app

SOURCES += main.cpp \
    speed.cpp \
    calendrier.cpp \
    particle.cpp \
    emetteur.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    speed.h \
    calendrier.h \
    particle.h \
    emetteur.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = .
INSTALLS += target

DISTFILES +=
