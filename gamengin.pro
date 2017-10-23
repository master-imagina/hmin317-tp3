#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T21:49:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gamengin
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
	main.cpp \
	src/mainwindow.cpp \
	src/glwidget.cpp \
        src/syncronizer.cpp \
    src/gameobject/gameobject.cpp \
    src/gameobject/component.cpp \
    src/gameobject/transform.cpp \
    src/gameobject/geometry.cpp \
    src/gameobject/particle_system.cpp \
    src/geometry/mesh.cpp \
    src/geometry/shapes.cpp \
    src/gameobject/material.cpp \
    src/gameobject/rigidbody.cpp \
    src/gameobject/camera.cpp \
    src/controller/camera_rts.cpp \
    src/controller/camera_facing.cpp

HEADERS += \
	src/mainwindow.h \
	src/glwidget.h \
        src/syncronizer.h \
    src/gameobject/gameobject.h \
    src/gameobject/transform.h \
    src/gameobject/component.h \
    src/gameobject/geometry.h \
    src/gameobject/particle_system.h \
    src/geometry/mesh.h \
    src/geometry/shapes.h \
    src/gameobject/material.h \
    src/gameobject/rigidbody.h \
    src/gameobject/camera.h \
    src/controller/camera_rts.h \
    src/controller/camera_facing.h

FORMS += \
	mainwindow.ui

