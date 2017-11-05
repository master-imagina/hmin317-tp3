project(3rdparty_entityx)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    3rdparty/catch.hpp
    3rdparty/simplesignal.h
    deps/Dependencies.h
    help/NonCopyable.h
    help/Pool.h
    help/Timer.h
    tags/TagsComponent.h
    config.h
    Entity.h
    entityx.h
    Event.h
    quick.h
    System.h
    )

set(SOURCES
    help/Pool.cc
    help/Timer.cc
    Entity.cc
    Event.cc
    System.cc
    )

# Targets
add_library(entityx SHARED ${SOURCES} ${HEADERS})
target_include_directories(entityx PUBLIC ${PROJECT_SOURCE_DIR}/..)
target_compile_definitions(entityx PRIVATE)
