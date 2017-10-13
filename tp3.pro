QT       += core gui widgets

TARGET = tp3
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp \
    syncronizer.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    syncronizer.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = .
INSTALLS += target
