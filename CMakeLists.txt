cmake_minimum_required(VERSION 3.5)

project(gameengines_pw)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    core/aliases_int.h
    core/aliases_memory.h
    core/gameloop.h
    render/aabb.h
    render/camera.h
    render/geometry.h
    render/material.h
    render/renderpass.h
    render/scene.h
    render/shaderparam.h
    render/shaderprogram.h
    render/shaderutils.h
    render/vertexattrib.h
    render/vertexlayout.h
    render/renderer/buffermanager.h
    render/renderer/drawcommand.h
    render/renderer/glbuffer.h
    render/renderer/glwrapper.h
    render/renderer/openglversion.h
    render/renderer/renderer.h
    render/renderer/shadermanager.h
    extras/cameraactions.h
    extras/cameracontroller.h
    extras/heightmap.h
    extras/particleeffect.h
    lab/seasons/mainwindow.h
    lab/seasons/seasoncontroller.h
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/dragtoolbutton.h
    editor/gamewidget.h
)

set(SOURCES
    core/gameloop.cpp
    render/material.cpp
    render/renderpass.cpp
    render/shaderparam.cpp
    render/shaderprogram.cpp
    render/shaderutils.cpp
    render/vertexattrib.cpp
    render/vertexlayout.cpp
    render/renderer/buffermanager.cpp
    render/renderer/glwrapper.cpp
    render/renderer/renderer.cpp
    render/renderer/shadermanager.cpp
    extras/cameraactions.cpp
    extras/cameracontroller.cpp
    extras/heightmap.cpp
    render/camera.cpp
    lab/seasons/mainwindow.cpp
    lab/seasons/main.cpp
    lab/seasons/seasoncontroller.cpp
    render/aabb.cpp
    editor/gamewidget.cpp
    editor/cameracontrols.cpp
    editor/dragtoolbutton.cpp
    extras/particleeffect.cpp
    editor/cameracontrollercontrols.cpp
    )

set(QT_RESOURCES
    lab/seasons/shaders.qrc
    editor/editor_resources.qrc)

# Copy <source>/res/images to <build>/
file(INSTALL lab/seasons/res/images DESTINATION ${CMAKE_BINARY_DIR})

# Packages
#  Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

# Targets
include_directories(${CMAKE_SOURCE_DIR})
add_executable(pw3 ${SOURCES} ${HEADERS} ${QT_RESOURCES})
target_link_libraries(pw3 Qt5::Widgets)
