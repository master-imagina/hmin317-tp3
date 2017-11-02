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
    render/scene.h
    render/geometry/geometry.h
    render/geometry/vertexattrib.h
    render/geometry/vertexlayout.h
    render/material/material.h
    render/material/renderpass.h
    render/material/shaderparam.h
    render/material/shaderprogram.h
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
    extras/particlematerial.h
    extras/shaderutils.h
    lab/seasons/mainwindow.h
    lab/seasons/seasoncontroller.h
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/dragtoolbutton.h
    editor/gamewidget.h
)

set(SOURCES
    core/gameloop.cpp
    render/camera.cpp
    render/geometry/vertexattrib.cpp
    render/geometry/vertexlayout.cpp
    render/renderer/buffermanager.cpp
    render/material/material.cpp
    render/material/renderpass.cpp
    render/material/shaderparam.cpp
    render/material/shaderprogram.cpp
    render/renderer/glwrapper.cpp
    render/renderer/renderer.cpp
    render/renderer/shadermanager.cpp
    extras/cameraactions.cpp
    extras/cameracontroller.cpp
    extras/heightmap.cpp
    extras/particleeffect.cpp
    extras/particlematerial.cpp
    extras/shaderutils.cpp
    lab/seasons/mainwindow.cpp
    lab/seasons/main.cpp
    lab/seasons/seasoncontroller.cpp
    render/aabb.cpp
    editor/gamewidget.cpp
    editor/cameracontrols.cpp
    editor/dragtoolbutton.cpp
    editor/cameracontrollercontrols.cpp
    )

set(QT_RESOURCES
    extras/extras_shaders.qrc
    lab/seasons/seasons_shaders.qrc
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
