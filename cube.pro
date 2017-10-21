QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    camerafirst.cpp \
    particulessystem.cpp \
    particlesrenderer.cpp \
    terraineffect.cpp

INCLUDEPATH+= $$PWD/external/glm/

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp\
    camera.cpp\
    camerathird.cpp\
    utils.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h\
    camera.h\
    camerathird.h\
    utils.h \
    camerafirst.h \
    particulessystem.h \
    particlesrenderer.h \
    terraineffect.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
