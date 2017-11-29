project(3rdparty_lua-5-3-4)

# C++
set(CMAKE_CXX_STANDARD 14)

# Files
file(GLOB LUA_HEADERS
     src/*.h
     )

file(GLOB LUA_SOURCES
     src/*.c
     )

# Targets
add_library(lua SHARED ${LUA_SOURCES} ${LUA_HEADERS})
target_include_directories(lua PUBLIC ${PROJECT_SOURCE_DIR}/..)
add_definitions(-DLUA_USE_POSIX=1)
target_compile_definitions(lua PRIVATE)
install(TARGETS lua LIBRARY DESTINATION ${ENGINESPW_LIB_INSTALL_PATH})

file(INSTALL ${LUA_HEADERS} DESTINATION ${PROJECT_BINARY_DIR}/include/lua5.3.4)
