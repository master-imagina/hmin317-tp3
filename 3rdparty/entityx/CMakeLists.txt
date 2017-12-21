cmake_minimum_required(VERSION 3.5)

project(entityx)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    entityx/3rdparty/catch.hpp
    entityx/3rdparty/simplesignal.h
    entityx/deps/Dependencies.h
    entityx/help/NonCopyable.h
    entityx/help/Pool.h
    entityx/help/Timer.h
    entityx/tags/TagsComponent.h
    entityx/config.h
    entityx/Entity.h
    entityx/entityx.h
    entityx/Event.h
    entityx/quick.h
    entityx/System.h
    )

set(SOURCES
    entityx/help/Pool.cc
    entityx/help/Timer.cc
    entityx/Entity.cc
    entityx/Event.cc
    entityx/System.cc
    )

install(
    DIRECTORY "entityx"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING PATTERN "*.h"
)

# Targets
add_library(entityx SHARED ${SOURCES} ${HEADERS})
target_include_directories(entityx PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_definitions(entityx PRIVATE)
install(TARGETS entityx LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
