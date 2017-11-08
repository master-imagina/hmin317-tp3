QT       += core gui widgets
QT       += 3dextras

TARGET = plane
TEMPLATE = app

SOURCES += main.cpp \
    particle.cpp \
    particleemitter.cpp \
    camera.cpp \
    firstpersoncamera.cpp \
    inputcontroller.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    particle.h \
    particleemitter.h \
    camera.h \
    firstpersoncamera.h \
    inputcontroller.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = .
INSTALLS += target

DISTFILES +=
LIBS += -lopengl32
