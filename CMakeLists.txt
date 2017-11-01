cmake_minimum_required(VERSION 3.5)

project(gameengines_pw3)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    aliases_int.h
    aabb.h
    cameraactions.h
    gamewidget.h
    geometry.h
    heightmap.h
    camera.h
    cameracontroller.h
    gameloop.h
    mainwindow.h
    particleeffect.h
    scene.h
    seasoncontroller.h
    cameracontrollercontrols.h
    cameracontrols.h
    dragtoolbutton.h
    shaderutils.h
    renderer/aliases_memory.h
    renderer/buffermanager.h
    renderer/openglversion.h
    renderer/renderer.h
    renderer/glbuffer.h
    renderer/vertexattrib.h
    renderer/vertexlayout.h
    renderer/glwrapper.h
    renderer/drawcommand.h
    renderer/material.h
    renderer/renderpass.h
    renderer/shaderparam.h
    renderer/shaderprogram.h
    renderer/shadermanager.h
)

set(SOURCES
    aabb.cpp
    cameraactions.cpp
    gamewidget.cpp
    heightmap.cpp
    camera.cpp
    cameracontroller.cpp
    cameracontrols.cpp
    dragtoolbutton.cpp
    mainwindow.cpp
    gameloop.cpp
    particleeffect.cpp
    seasoncontroller.cpp
    cameracontrollercontrols.cpp
    shaderutils.cpp
    main.cpp
    renderer/buffermanager.cpp
    renderer/renderer.cpp
    renderer/vertexattrib.cpp
    renderer/vertexlayout.cpp
    renderer/glwrapper.cpp
    renderer/material.cpp
    renderer/renderpass.cpp
    renderer/shaderparam.cpp
    renderer/shaderprogram.cpp
    renderer/shadermanager.cpp
    )

set(QT_RESOURCES
    shaders.qrc
    editor_resources.qrc)

# Copy <source>/res/images to <build>/
file(INSTALL res/images DESTINATION ${CMAKE_BINARY_DIR})

# Packages
#  Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

#  Binary
add_executable(pw3 ${SOURCES} ${HEADERS} ${QT_RESOURCES})
target_link_libraries(pw3 Qt5::Widgets)
