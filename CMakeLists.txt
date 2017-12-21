cmake_minimum_required(VERSION 3.5)

project(gameengines_pw)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# Project constants
set (ENGINESPW_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set (ENGINESPW_BUILD_DIR ${CMAKE_BINARY_DIR})

# Install directories
set(ENGINESPW_INSTALL_PATH ${ENGINESPW_BUILD_DIR}/install)
set(CMAKE_INSTALL_PREFIX ${ENGINESPW_INSTALL_PATH})

set(ENGINESPW_BIN_INSTALL_PATH ${ENGINESPW_INSTALL_PATH})
set(ENGINESPW_LIB_INSTALL_PATH ${ENGINESPW_INSTALL_PATH}/lib)
set(ENGINESPW_INCLUDE_INSTALL_PATH ${ENGINESPW_INSTALL_PATH}/include)


# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_EMIT -Wno-deprecated-declarations")


# Subdirs
add_subdirectory(3rdparty/luabridge)

ExternalProject_add(3rdparty_entityx
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/entityx
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/entityx
    # Forward CMAKE_INSTALL_PREFIX to external project.
    CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ENGINESPW_INSTALL_PATH}
)

set(ENABLED_ASSIMP_FORMATS
    BLEND
    OBJ
    OFF
    PLY)

set(ASSIMP_CMAKE_FORMATS_ARGS "")

foreach(format ${ENABLED_ASSIMP_FORMATS})
    set(ASSIMP_CMAKE_FORMATS_ARGS ${ASSIMP_CMAKE_FORMATS_ARGS} -DASSIMP_BUILD_${format}_IMPORTER:BOOL=true)
endforeach(format)

ExternalProject_add(3rdparty_assimp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/assimp-4.0.1
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/assimp-4.0.1
    # Forward CMAKE_INSTALL_PREFIX to external project.
    CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ENGINESPW_INSTALL_PATH}
                     -DASSIMP_INCLUDE_INSTALL_DIR:STRING=${ENGINESPW_INCLUDE_INSTALL_PATH}
                     -DASSIMP_NO_EXPORT:BOOL=true
                     -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT:BOOL=false
                     ${ASSIMP_CMAKE_FORMATS_ARGS}
)

ExternalProject_add(3rdparty_bullet
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bullet3-2.87
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/bullet3-2.87
    # Forward CMAKE_INSTALL_PREFIX to external project.
    CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ENGINESPW_INSTALL_PATH}
                     -DINCLUDE_INSTALL_DIR:STRING=${ENGINESPW_INCLUDE_INSTALL_PATH}
)

ExternalProject_add(3rdparty_lua
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lua-5.3.4
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/lua-5.3.4
    # Forward CMAKE_INSTALL_PREFIX to external project.
    CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ENGINESPW_INSTALL_PATH}
                     -DLUA_INCLUDE_INSTALL_DIR:STRING=${ENGINESPW_INCLUDE_INSTALL_PATH}
)

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
    core/param.h
    core/scene.h
    core/systemengine.h
    )
set(CORE_SOURCES
    core/assetmanager.cpp
    core/bigfile.cpp
    core/gameloop.cpp
    core/scene.cpp
    core/param.cpp
    core/systemengine.cpp
    )

link_directories(${ENGINESPW_LIB_INSTALL_PATH})

#set(CMAKE_INSTALL_RPATH ${ENGINESPW_LIB_INSTALL_PATH})
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(enginecore SHARED ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(enginecore PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(enginecore PRIVATE CORE_LIBRARY=1)
target_link_libraries(enginecore Qt5::Core Qt5::Gui entityx)

add_dependencies(enginecore 3rdparty_entityx)

install(TARGETS enginecore LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  render
set(RENDER_HEADERS
    render/aabb.h
    render/renderassets.h
    render/camera.h
    render/light.h
    render/mesh.h
    render/rendersystem.h
    render/transform.h
    render/renderwidget.h
    render/geometry/geometry.h
    render/geometry/vertexattrib.h
    render/geometry/vertexlayout.h
    render/material/builtins.h
    render/material/material.h
    render/material/renderpass.h
    render/material/shaderprogram.h
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
    render/renderassets.cpp
    render/camera.cpp
    render/light.cpp
    render/mesh.cpp
    render/rendersystem.cpp
    render/transform.cpp
    render/renderwidget.cpp
    render/geometry/geometry.cpp
    render/geometry/vertexattrib.cpp
    render/geometry/vertexlayout.cpp
    render/renderer/managers/buffermanager.cpp
    render/renderer/managers/shadermanager.cpp
    render/renderer/managers/texturemanager.cpp
    render/renderer/managers/vaomanager.cpp
    render/material/builtins.cpp
    render/material/material.cpp
    render/material/renderpass.cpp
    render/material/shaderprogram.cpp
    render/renderer/glwrapper.cpp
    render/renderer/renderer.cpp
    )
file(GLOB RENDER_SHADERS
    render/shaders/*.vert
    render/shaders/*.geom
    render/shaders/*.frag)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/builtins/shaders)
file(COPY render/shaders DESTINATION ${CMAKE_BINARY_DIR}/builtins)
file(INSTALL render/shaders DESTINATION ${ENGINESPW_INSTALL_PATH}/builtins)

add_library(enginerender SHARED ${RENDER_SOURCES} ${RENDER_HEADERS} ${RENDER_SHADERS})
target_include_directories(enginerender PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(enginerender PRIVATE RENDER_LIBRARY=1)
target_link_libraries(enginerender enginecore Qt5::Gui Qt5::Widgets entityx assimp)

add_dependencies(enginerender enginecore 3rdparty_assimp)

install(TARGETS enginerender LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  input
set(INPUT_HEADERS
    input/eventhook.h
    input/inputsystem.h
    input/keyboard.h
    )
set(INPUT_SOURCES
    input/eventhook.cpp
    input/inputsystem.cpp
    )

add_library(engineinput SHARED ${INPUT_SOURCES} ${INPUT_HEADERS})
target_include_directories(engineinput PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(engineinput PRIVATE INPUT_LIBRARY=1)
target_link_libraries(engineinput Qt5::Core Qt5::Gui entityx)

add_dependencies(engineinput enginecore)

install(TARGETS engineinput LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  physics
set(3RDPARTY_BULLET_INCLUDE_DIRS
    ${ENGINESPW_INSTALL_PATH}/include/bullet
    )

set(PHYSICS_HEADERS
    physics/bullet_engine_bridge.h
    physics/collider.h
    physics/collisionsystem.h
    physics/rigidbody.h
    )
set(PHYSICS_SOURCES
    physics/bullet_engine_bridge.cpp
    physics/collider.cpp
    physics/collisionsystem.cpp
    physics/rigidbody.cpp
    )

add_library(enginephysics SHARED ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})
target_include_directories(enginephysics PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(enginephysics PRIVATE PHYSICS_LIBRARY=1)
target_link_libraries(enginephysics enginecore enginerender Qt5::Core entityx BulletCollision BulletDynamics)

add_dependencies(enginephysics 3rdparty_bullet)

install(TARGETS enginephysics LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  script
set(SCRIPT_HEADERS
    script/api.h
    script/lua_includes.h
    script/luaqvariant_bridge.h
    script/luaserver.h
    script/script.h
    script/scriptassets.h
    script/scriptsystem.h
    )
set(SCRIPT_SOURCES
    script/api.cpp
    script/luaqvariant_bridge.cpp
    script/luaserver.cpp
    script/script.cpp
    script/scriptsystem.cpp
    script/scriptassets.cpp
    )

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/builtins/scripts)
file(COPY script/scripts DESTINATION ${CMAKE_BINARY_DIR}/builtins)
file(INSTALL script/scripts DESTINATION ${ENGINESPW_INSTALL_PATH}/builtins)

file(GLOB SCRIPTS_BUILTINS
    script/scripts/*.lua)

add_library(enginescript SHARED ${SCRIPT_SOURCES} ${SCRIPT_HEADERS} ${SCRIPTS_BUILTINS})
target_include_directories(enginescript PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(enginescript PRIVATE SCRIPT_LIBRARY=1)
target_link_libraries(enginescript enginecore enginerender enginephysics Qt5::Core entityx lua)

add_dependencies(enginescript enginecore enginephysics 3rdparty_lua)

install(TARGETS enginescript LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

#  extras
set(EXTRAS_HEADERS
    extras/cameraactions.h
    extras/grid.h
    extras/heightmap.h
    extras/particles/particleeffect.h
    extras/particles/particlesystem.h
    extras/particles/quick.h
    extras/serialization.h
    )
set(EXTRAS_SOURCES
    extras/cameraactions.cpp
    extras/grid.cpp
    extras/heightmap.cpp
    extras/particles/particleeffect.cpp
    extras/particles/particlesystem.cpp
    extras/particles/quick.cpp
    extras/serialization.cpp
    )
set(EXTRAS_RESOURCES
    )
file(GLOB EXTRAS_SHADERS
    extras/shaders/*.vert
    extras/shaders/*.geom
    extras/shaders/*.frag)

file(COPY extras/shaders DESTINATION ${CMAKE_BINARY_DIR}/builtins)
file(INSTALL extras/shaders DESTINATION ${ENGINESPW_INSTALL_PATH}/builtins)

add_library(engineextras SHARED ${EXTRAS_SOURCES} ${EXTRAS_HEADERS} ${EXTRAS_RESOURCES} ${EXTRAS_SHADERS})
target_include_directories(engineextras PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(engineextras PRIVATE EXTRAS_LIBRARY=1)
target_link_libraries(engineextras enginecore enginerender engineinput enginescript Qt5::Core Qt5::Gui entityx)

add_dependencies(engineextras enginecore engineinput enginescript)

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
    editor/gui/gamewidget.h
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
    editor/gui/gamewidget.cpp
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
target_include_directories(editorgui PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_compile_definitions(editorgui PRIVATE EDITORGUI_LIBRARY=1)
target_link_libraries(editorgui enginecore enginephysics Qt5::Widgets)

add_dependencies(editorgui engineextras)

install(TARGETS editorgui LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

## RPath
#set(CMAKE_INSTALL_RPATH "lib")
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#  editor
set(EDITOR_HEADERS
    editor/assetmanagerview.h
    editor/cameracontrols.h
    editor/cameracontrollercontrols.h
    editor/componentuihandlers.h
    editor/componentview.h
    editor/entityxhook.h
    editor/entitylistmodel.h
    editor/mainwindow.h
    editor/particleeditor.h
    editor/projectmanager.h
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
    editor/entityxhook.cpp
    editor/entitylistmodel.cpp
    editor/main.cpp
    editor/mainwindow.cpp
    editor/particleeditor.cpp
    editor/projectmanager.cpp
    editor/sceneview.cpp
    editor/panemanager.cpp
    )
set(EDITOR_RESOURCES
    )

add_executable(editor ${EDITOR_SOURCES} ${EDITOR_HEADERS} ${EDITOR_RESOURCES})
target_include_directories(editor PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_link_libraries(editor editorgui engineextras enginescript engineinput enginerender enginecore Qt5::Widgets entityx)

add_dependencies(editor editorgui)

install(TARGETS editor RUNTIME DESTINATION ${ENGINESPW_BIN_INSTALL_PATH})


# Game data cmake helpers
set(GAME_DATA_TREE
    images
    meshes
    shaders
    scripts
    )

macro (copyGameData sourcePath destinationDir)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${destinationDir})

    foreach(gameDataFolder ${GAME_DATA_TREE})
        message(GNEUUUUU ${CMAKE_SOURCE_DIR}/${sourcePath}/data/${gameDataFolder})
        if (EXISTS ${CMAKE_SOURCE_DIR}/${sourcePath}/data/${gameDataFolder})
            file(COPY ${sourcePath}/data/${gameDataFolder} DESTINATION ${CMAKE_BINARY_DIR}/${destinationDir})
            file(INSTALL ${sourcePath}/data/${gameDataFolder} DESTINATION ${ENGINESPW_INSTALL_PATH}/${destinationDir})
        endif()
    endforeach(gameDataFolder)
endmacro(copyGameData)


#  seasons
set(SEASONS_HEADERS
    lab/seasons/seasoncontroller.h
    )
set(SEASONS_SOURCES
    lab/seasons/main.cpp
    lab/seasons/seasoncontroller.cpp
    )
file(GLOB SEASONS_SHADERS
    lab/seasons/data/shaders/*.vert
    lab/seasons/data/shaders/*.geom
    lab/seasons/data/shaders/*.frag
    )
file(GLOB SEASONS_SCRIPTS
    lab/seasons/data/scripts/*.lua)

copyGameData(lab/seasons data_seasons)

add_executable(seasons ${SEASONS_SOURCES} ${SEASONS_HEADERS} ${SEASONS_SHADERS} ${SEASONS_SCRIPTS})
target_include_directories(seasons PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_link_libraries(seasons enginescript engineinput editorgui engineextras enginerender enginecore editorgui Qt5::Widgets entityx)

install(TARGETS seasons RUNTIME DESTINATION ${ENGINESPW_BIN_INSTALL_PATH})

#  marble
set(MARBLE_HEADERS
    )
set(MARBLE_SOURCES
    lab/marble/main.cpp
    )
file(GLOB MARBLE_SHADERS
    lab/marble/data/shaders/*.vert
    lab/marble/data/shaders/*.geom
    lab/marble/data/shaders/*.frag
    )
file(GLOB MARBLE_SCRIPTS
    lab/marble/data/scripts/*.lua)

copyGameData(lab/marble data_marble)

add_executable(marble ${MARBLE_SOURCES} ${MARBLE_HEADERS} ${MARBLE_SHADERS} ${MARBLE_SCRIPTS})
target_include_directories(marble PRIVATE ${PROJECT_SOURCE_DIR} ${ENGINESPW_INCLUDE_INSTALL_PATH})
target_link_libraries(marble enginescript engineinput editorgui engineextras enginerender enginecore editorgui Qt5::Widgets entityx)

install(TARGETS marble RUNTIME DESTINATION ${ENGINESPW_BIN_INSTALL_PATH})
