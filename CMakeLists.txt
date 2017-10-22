cmake_minimum_required(VERSION 3.5)

project(gameengines_pw3)

# C++ language version
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    aabb.h
    cameraactions.h
    gamewidget.h
    geometry.h
    heightmap.h
    renderer.h
    camera.h
    cameracontroller.h
    gameloop.h
    mainwindow.h
    particleeffect.h
    renderer/openglversion.h
    renderer/buffer.h
    renderer/vertexattrib.h
    renderer/geometrydesc.h
    )

set(SOURCES
    aabb.cpp
    cameraactions.cpp
    gamewidget.cpp
    geometry.cpp
    heightmap.cpp
    renderer.cpp
    camera.cpp
    cameracontroller.cpp
    mainwindow.cpp
    gameloop.cpp
    particleeffect.cpp
    main.cpp
    renderer/buffer.cpp
    renderer/vertexattrib.cpp
    renderer/geometrydesc.cpp
    )

set(QT_RESOURCES
    shaders.qrc)

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
