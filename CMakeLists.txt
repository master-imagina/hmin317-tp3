cmake_minimum_required(VERSION 3.5)

project(gameengines_pw)

# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_EMIT")

# Files
set(HEADERS
    core/aliases_int.h
    core/aliases_memory.h
    core/gameloop.h
    core/scene.h
    core/systemengine.h
    render/aabb.h
    render/camera.h
    render/rendersystem.h
    render/transform.h
    render/geometry/geometry.h
    render/geometry/vertexattrib.h
    render/geometry/vertexlayout.h
    render/material/material.h
    render/material/renderpass.h
    render/material/shaderparam.h
    render/material/shaderprogram.h
    render/material/shaderutils.h
    render/renderer/buffermanager.h
    render/renderer/drawcommand.h
    render/renderer/glbuffer.h
    render/renderer/glwrapper.h
    render/renderer/openglversion.h
    render/renderer/renderer.h
    render/renderer/shadermanager.h
    render/renderer/vaomanager.h
    extras/cameraactions.h
    extras/cameracontroller.h
    extras/heightmap.h
    extras/particles/particleeffect.h
    extras/particles/particlesystem.h
    extras/particles/quick.h
    lab/seasons/seasoncontroller.h
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/dragtoolbutton.h
    editor/fpswidgets.h
    editor/mainwindow.h
    editor/renderwidget.h
    )

set(SOURCES
    core/gameloop.cpp
    core/scene.cpp
    core/systemengine.cpp
    render/camera.cpp
    render/rendersystem.cpp
    render/transform.cpp
    render/geometry/vertexattrib.cpp
    render/geometry/vertexlayout.cpp
    render/renderer/buffermanager.cpp
    render/material/material.cpp
    render/material/renderpass.cpp
    render/material/shaderparam.cpp
    render/material/shaderprogram.cpp
    render/material/shaderutils.cpp
    render/renderer/glwrapper.cpp
    render/renderer/renderer.cpp
    render/renderer/shadermanager.cpp
    render/renderer/vaomanager.cpp
    extras/cameraactions.cpp
    extras/cameracontroller.cpp
    extras/heightmap.cpp
    extras/particles/particleeffect.cpp
    extras/particles/particlesystem.cpp
    extras/particles/quick.cpp
    lab/seasons/main.cpp
    lab/seasons/seasoncontroller.cpp
    render/aabb.cpp
    editor/cameracontrols.cpp
    editor/cameracontrollercontrols.cpp
    editor/dragtoolbutton.cpp
    editor/fpswidgets.cpp
    editor/mainwindow.cpp
    editor/renderwidget.cpp
    )

set(QT_RESOURCES
    extras/particles/extras_particles_shaders.qrc
    lab/seasons/seasons_shaders.qrc
    editor/editor_resources.qrc)

# Copy <source>/res/images to <build>/
file(INSTALL lab/seasons/res/images DESTINATION ${CMAKE_BINARY_DIR})

# Packages
#  Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

# Subdirs
add_subdirectory(3rdparty)

# Targets
add_executable(pw3 ${SOURCES} ${HEADERS} ${QT_RESOURCES})
target_include_directories(pw3 PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(pw3 Qt5::Widgets entityx)
