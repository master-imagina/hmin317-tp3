cmake_minimum_required(VERSION 3.5)

project(gameengines_pw)

# Project constants
set (ENGINESPW_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set (ENGINESPW_BUILD_DIR ${CMAKE_BINARY_DIR})

# Install directories
set(ENGINESPW_INSTALL_PATH ${ENGINESPW_BUILD_DIR}/install)
set(CMAKE_INSTALL_PREFIX ${ENGINESPW_INSTALL_PATH})

set(ENGINESPW_LIB_INSTALL_DIR lib)

set(ENGINESPW_BIN_INSTALL_PATH ${ENGINESPW_INSTALL_PATH})
set(ENGINESPW_LIB_INSTALL_PATH ${ENGINESPW_INSTALL_PATH}/lib)


# Configure assimp
#  Don't pollute the the install dir with public headers
set(ASSIMP_INCLUDE_INSTALL_DIR ${PROJECT_BINARY_DIR}/USELESS/include CACHE STRING "" FORCE)

#  Don't build exporters, we only need importers
set(ASSIMP_NO_EXPORT true CACHE BOOL "" FORCE)
#  Choose importers
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT false CACHE BOOL "" FORCE)

set(ENABLED_ASSIMP_FORMATS
    BLEND
    OBJ
    OFF
    PLY)

foreach(format ${ENABLED_ASSIMP_FORMATS})
    set(ASSIMP_BUILD_${format}_IMPORTER true CACHE BOOL "" FORCE)
endforeach(format)

#  Retrieve include directories
set(3RDPARTY_ASSIMP_INCLUDE_DIRS
    3rdparty/assimp-4.0.1/include
    ${PROJECT_BINARY_DIR}/3rdparty/assimp-4.0.1/include
    )


# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_EMIT")


# Subdirs
add_subdirectory(3rdparty)


# Packages
#  Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)


# Targets
#  core
set(CORE_HEADERS
    core/aliases_int.h
    core/aliases_memory.h
    core/assetmanager.h
    core/bigfile.h
    core/gameloop.h
    core/log.h
    core/scene.h
    core/systemengine.h
    )
set(CORE_SOURCES
    core/assetmanager.cpp
    core/bigfile.cpp
    core/gameloop.cpp
    core/scene.cpp
    core/systemengine.cpp
    )

add_library(enginecore SHARED ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(enginecore PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(enginecore PRIVATE CORE_LIBRARY=1)
target_link_libraries(enginecore Qt5::Core Qt5::Gui entityx)

install(TARGETS enginecore LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

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
    render/renderer/managers/buffermanager.h
    render/renderer/managers/shadermanager.h
    render/renderer/managers/texturemanager.h
    render/renderer/managers/vaomanager.h
    render/renderer/drawcommand.h
    render/renderer/glbuffer.h
    render/renderer/gltexture.h
    render/renderer/glshaderprogram.h
    render/renderer/glvao.h
    render/renderer/glwrapper.h
    render/renderer/openglversion.h
    render/renderer/renderer.h
    )
set(RENDER_SOURCES
    render/aabb.cpp
    render/camera.cpp
    render/rendersystem.cpp
    render/transform.cpp
    render/renderwidget.cpp
    render/geometry/vertexattrib.cpp
    render/geometry/vertexlayout.cpp
    render/renderer/managers/buffermanager.cpp
    render/renderer/managers/shadermanager.cpp
    render/renderer/managers/texturemanager.cpp
    render/renderer/managers/vaomanager.cpp
    render/material/material.cpp
    render/material/renderpass.cpp
    render/material/shaderparam.cpp
    render/material/shaderprogram.cpp
    render/material/shaderutils.cpp
    render/renderer/glwrapper.cpp
    render/renderer/renderer.cpp
    )

add_library(enginerender SHARED ${RENDER_SOURCES} ${RENDER_HEADERS})
target_include_directories(enginerender PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(enginerender PRIVATE RENDER_LIBRARY=1)
target_link_libraries(enginerender enginecore Qt5::Gui Qt5::Widgets entityx)

install(TARGETS enginerender LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  extras
set(EXTRAS_HEADERS
    extras/cameraactions.h
    extras/cameracontroller.h
    extras/gamewidget.h
    extras/grid.h
    extras/heightmap.h
    extras/particles/particleeffect.h
    extras/particles/particlesystem.h
    extras/particles/quick.h
    )
set(EXTRAS_SOURCES
    extras/cameraactions.cpp
    extras/cameracontroller.cpp
    extras/gamewidget.cpp
    extras/grid.cpp
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
file(INSTALL extras/shaders DESTINATION ${ENGINESPW_INSTALL_PATH})

add_library(engineextras SHARED ${EXTRAS_SOURCES} ${EXTRAS_HEADERS} ${EXTRAS_RESOURCES} ${EXTRAS_SHADERS})
target_include_directories(engineextras PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_definitions(engineextras PRIVATE EXTRAS_LIBRARY=1)
target_link_libraries(engineextras enginecore enginerender Qt5::Core Qt5::Gui entityx)

install(TARGETS engineextras LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  editor.gui
set(EDITORGUI_HEADERS
    editor/gui/advancedslider.h
    editor/gui/coloreditor/coloreditor.h
    editor/gui/coloreditor/hueslider.h
    editor/gui/coloreditor/saturationslider.h
    editor/gui/coloreditor/valueslider.h
    editor/gui/dragtoolbutton.h
    editor/gui/flowlayout.h
    editor/gui/fpswidgets.h
    editor/gui/layoututils.h
    editor/gui/pane.h
    editor/gui/urledit.h
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
    editor/gui/flowlayout.cpp
    editor/gui/fpswidgets.cpp
    editor/gui/layoututils.cpp
    editor/gui/pane.cpp
    editor/gui/urledit.cpp
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

install(TARGETS editorgui LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

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

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data_seasons)
file(COPY lab/seasons/data/images DESTINATION ${CMAKE_BINARY_DIR}/data_seasons)
file(INSTALL lab/seasons/data/images DESTINATION ${ENGINESPW_INSTALL_PATH}/data_seasons)
file(COPY lab/seasons/data/meshes DESTINATION ${CMAKE_BINARY_DIR}/data_seasons)
file(INSTALL lab/seasons/data/meshes DESTINATION ${ENGINESPW_INSTALL_PATH}/data_seasons)

add_executable(seasons ${SEASONS_SOURCES} ${SEASONS_HEADERS})
target_include_directories(seasons PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(seasons editorgui engineextras enginerender enginecore Qt5::Widgets entityx)

install(TARGETS seasons RUNTIME DESTINATION ${ENGINESPW_BIN_INSTALL_PATH})

#  editor
set(EDITOR_HEADERS
    editor/assetmanagerview.h
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/componentuihandlers.h
    editor/componentview.h
    editor/hooksystems.h
    editor/mainwindow.h
    editor/particleeditor.h
    editor/sceneview.h
    editor/panemanager.h
    editor/icomponentuihandler.h
    )
set(EDITOR_SOURCES
    editor/assetmanagerview.cpp
    editor/cameracontrols.cpp
    editor/cameracontrollercontrols.cpp
    editor/componentuihandlers.cpp
    editor/componentview.cpp
    editor/hooksystems.cpp
    editor/main.cpp
    editor/mainwindow.cpp
    editor/particleeditor.cpp
    editor/sceneview.cpp
    editor/panemanager.cpp
    )
set(EDITOR_RESOURCES
    )

file(COPY editor/res/images DESTINATION ${CMAKE_BINARY_DIR})
file(INSTALL editor/res/images DESTINATION ${ENGINESPW_INSTALL_PATH})

add_executable(editor ${EDITOR_SOURCES} ${EDITOR_HEADERS} ${EDITOR_RESOURCES})
target_include_directories(editor PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(editor editorgui engineextras enginerender enginecore Qt5::Widgets entityx)

install(TARGETS editor RUNTIME DESTINATION ${ENGINESPW_BIN_INSTALL_PATH})
