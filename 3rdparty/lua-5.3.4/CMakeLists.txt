cmake_minimum_required(VERSION 3.5)

project(lua-5-3-4)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
file(GLOB LUA_HEADERS
     src/*.h
     )

file(GLOB LUA_SOURCES
     src/*.c
     )
     
set(LUA_INCLUDE_INSTALL_DIR "include" CACHE STRING "")

# Targets
add_library(lua SHARED ${LUA_SOURCES} ${LUA_HEADERS})
target_include_directories(lua PUBLIC ${PROJECT_SOURCE_DIR}/..)
add_definitions(-DLUA_USE_POSIX=1)
target_compile_definitions(lua PRIVATE)
install(TARGETS lua LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(INSTALL ${LUA_HEADERS} DESTINATION ${LUA_INCLUDE_INSTALL_DIR}/lua5.3.4)
