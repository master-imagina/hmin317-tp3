cmake_minimum_required(VERSION 3.5)

project(gameengines_pw)

# Project constants
set (ENGINESPW_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set (ENGINESPW_BUILD_DIR ${CMAKE_BINARY_DIR})

# Install directories
set(ENGINESPW_INSTALL_DIR ${ENGINESPW_BUILD_DIR}/install)
set(CMAKE_INSTALL_PREFIX ${ENGINESPW_INSTALL_DIR})

set(ENGINESPW_BIN_DIR ${ENGINESPW_INSTALL_DIR})
set(ENGINESPW_LIBRARY_DIR ${ENGINESPW_INSTALL_DIR}/lib)


# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_EMIT")


# Packages
#  Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

# Subdirs
add_subdirectory(3rdparty)

# Targets
#  core
set(CORE_HEADERS
    core/aliases_int.h
    core/aliases_memory.h
    core/gameloop.h
    core/log.h
    core/scene.h
    core/systemengine.h
    )
set(CORE_SOURCES
    core/gameloop.cpp
    core/scene.cpp
    core/systemengine.cpp
    )

add_library(enginecore SHARED ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(enginecore PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(enginecore PRIVATE CORE_LIBRARY=1)
target_link_libraries(enginecore Qt5::Core entityx)

install(TARGETS enginecore LIBRARY DESTINATION ${ENGINESPW_LIBRARY_DIR})

#  render
set(RENDER_HEADERS
    render/aabb.h
    render/camera.h
    render/rendersystem.h
    render/transform.h
    render/renderwidget.h
    render/geometry/geometry.h
    render/geometry/vertexattrib.h
    render/geometry/vertexlayout.h
    render/material/material.h
    render/material/renderpass.h
    render/material/shaderparam.h
    render/material/shaderprogram.h
    render/material/shaderutils.h
    render/material/texture.h
    render/renderer/buffermanager.h
    render/renderer/drawcommand.h
    render/renderer/glbuffer.h
    render/renderer/gltexture.h
    render/renderer/glwrapper.h
    render/renderer/openglversion.h
    render/renderer/renderer.h
    render/renderer/shadermanager.h
    render/renderer/texturemanager.h
    render/renderer/vaomanager.h
    )
set(RENDER_SOURCES
    render/aabb.cpp
    render/camera.cpp
    render/rendersystem.cpp
    render/transform.cpp
    render/renderwidget.cpp
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
    render/renderer/texturemanager.cpp
    render/renderer/vaomanager.cpp
    )

add_library(enginerender SHARED ${RENDER_SOURCES} ${RENDER_HEADERS})
target_include_directories(enginerender PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(enginerender PRIVATE RENDER_LIBRARY=1)
target_link_libraries(enginerender enginecore Qt5::Gui Qt5::Widgets entityx)

install(TARGETS enginerender LIBRARY DESTINATION ${ENGINESPW_LIBRARY_DIR})

#  extras
set(EXTRAS_HEADERS
    extras/cameraactions.h
    extras/cameracontroller.h
    extras/gamewidget.h
    extras/heightmap.h
    extras/particles/particleeffect.h
    extras/particles/particlesystem.h
    extras/particles/quick.h
    )
set(EXTRAS_SOURCES
    extras/cameraactions.cpp
    extras/cameracontroller.cpp
    extras/gamewidget.cpp
    extras/heightmap.cpp
    extras/particles/particleeffect.cpp
    extras/particles/particlesystem.cpp
    extras/particles/quick.cpp
    )
set(EXTRAS_RESOURCES
    )
file(GLOB EXTRAS_SHADERS
    extras/shaders/*.vert
    extras/shaders/*.geom
    extras/shaders/*.frag)

file(COPY extras/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(INSTALL extras/shaders DESTINATION ${ENGINESPW_INSTALL_DIR})

add_library(engineextras SHARED ${EXTRAS_SOURCES} ${EXTRAS_HEADERS} ${EXTRAS_RESOURCES} ${EXTRAS_SHADERS})
target_include_directories(engineextras PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(engineextras PRIVATE EXTRAS_LIBRARY=1)
target_link_libraries(engineextras enginecore enginerender Qt5::Core Qt5::Gui entityx)

install(TARGETS engineextras LIBRARY DESTINATION ${ENGINESPW_LIBRARY_DIR})

#  editor.gui
set(EDITORGUI_HEADERS
    editor/gui/advancedslider.h
    editor/gui/coloreditor/coloreditor.h
    editor/gui/coloreditor/hueslider.h
    editor/gui/coloreditor/saturationslider.h
    editor/gui/coloreditor/valueslider.h
    editor/gui/dragtoolbutton.h
    editor/gui/fpswidgets.h
    editor/gui/vec3edit.h
    editor/gui/vecdimindicator.h
    )
set(EDITORGUI_SOURCES
    editor/gui/advancedslider.cpp
    editor/gui/coloreditor/coloreditor.cpp
    editor/gui/coloreditor/hueslider.cpp
    editor/gui/coloreditor/saturationslider.cpp
    editor/gui/coloreditor/valueslider.cpp
    editor/gui/dragtoolbutton.cpp
    editor/gui/fpswidgets.cpp
    editor/gui/vec3edit.cpp
    editor/gui/vecdimindicator.cpp
    )
set(EDITORGUI_RESOURCES
    editor/gui/gui_resources.qrc
    )

add_library(editorgui SHARED ${EDITORGUI_SOURCES} ${EDITORGUI_HEADERS} ${EDITORGUI_RESOURCES})
target_include_directories(editorgui PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(editorgui PRIVATE EDITORGUI_LIBRARY=1)
target_link_libraries(editorgui enginecore Qt5::Widgets)

install(TARGETS editorgui LIBRARY DESTINATION ${ENGINESPW_LIBRARY_DIR})

# RPath
set(CMAKE_INSTALL_RPATH "lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#  seasons
set(SEASONS_HEADERS
    lab/seasons/seasoncontroller.h
    )
set(SEASONS_SOURCES
    lab/seasons/main.cpp
    lab/seasons/seasoncontroller.cpp
    )
set(SEASONS_RESOURCES
    )

file(COPY lab/seasons/res/images DESTINATION ${CMAKE_BINARY_DIR})
file(INSTALL lab/seasons/res/images DESTINATION ${ENGINESPW_INSTALL_DIR})

add_executable(season ${SEASONS_SOURCES} ${SEASONS_HEADERS} ${SEASONS_RESOURCES})
target_include_directories(season PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(season editorgui engineextras enginerender enginecore Qt5::Widgets entityx)

install(TARGETS season RUNTIME DESTINATION ${ENGINESPW_BIN_DIR})

#  editor
set(EDITOR_HEADERS
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/mainwindow.h
    editor/particleeditor.h
    )
set(EDITOR_SOURCES
    editor/cameracontrols.cpp
    editor/cameracontrollercontrols.cpp
    editor/main.cpp
    editor/mainwindow.cpp
    editor/particleeditor.cpp
    )
set(EDITOR_RESOURCES
    )

file(COPY editor/res/images DESTINATION ${CMAKE_BINARY_DIR})
file(INSTALL editor/res/images DESTINATION ${ENGINESPW_INSTALL_DIR})

add_executable(editor ${EDITOR_SOURCES} ${EDITOR_HEADERS} ${EDITOR_RESOURCES})
target_include_directories(editor PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(editor editorgui engineextras enginerender enginecore Qt5::Widgets entityx)

install(TARGETS editor RUNTIME DESTINATION ${ENGINESPW_BIN_DIR})
