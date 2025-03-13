cmake_minimum_required(VERSION 3.14)
project(raylib_luajit LANGUAGES C CXX)

# Add CPM.cmake
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
    file(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake"
        "${CMAKE_BINARY_DIR}/cmake/CPM.cmake"
    )
endif()
include(${CMAKE_BINARY_DIR}/cmake/CPM.cmake)

# Add raylib 5.5 as a shared library
CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG 5.5
    OPTIONS
        "BUILD_EXAMPLES OFF"
        "BUILD_GAMES OFF"
        "BUILD_SHARED_LIBS ON"
)

# Add LuaJIT
CPMAddPackage(
    NAME luajit
    GITHUB_REPOSITORY LuaJIT/LuaJIT
    GIT_TAG v2.1.0-beta3
    DOWNLOAD_ONLY YES
)

# Build LuaJIT for Windows with MSVC
if(luajit_ADDED)
    set(LUAJIT_SRC_DIR ${luajit_SOURCE_DIR})
    set(LUAJIT_LIB "${LUAJIT_SRC_DIR}/src/luajit.lib")
    set(LUAJIT_LUA51_LIB "${LUAJIT_SRC_DIR}/src/lua51.lib")

    if(NOT EXISTS ${LUAJIT_LIB} OR NOT EXISTS ${LUAJIT_LUA51_LIB})
        set(VS_VCVARSALL "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat")
        add_custom_command(
            OUTPUT ${LUAJIT_LIB} ${LUAJIT_LUA51_LIB}
            COMMAND cmd /c "call \"${VS_VCVARSALL}\" x64 && cd /d ${LUAJIT_SRC_DIR}/src && call ${LUAJIT_SRC_DIR}/src/msvcbuild.bat static"
            DEPENDS ${LUAJIT_SRC_DIR}/src/luajit.c
            WORKING_DIRECTORY ${LUAJIT_SRC_DIR}/src
            COMMENT "Building LuaJIT static library with MSVC"
        )
        add_custom_target(luajit_build DEPENDS ${LUAJIT_LIB} ${LUAJIT_LUA51_LIB})
    else()
        message(STATUS "Using existing LuaJIT libraries: ${LUAJIT_LIB} and ${LUAJIT_LUA51_LIB}")
    endif()

    add_library(luajit STATIC IMPORTED)
    set_target_properties(luajit PROPERTIES
        IMPORTED_LOCATION "${LUAJIT_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${LUAJIT_SRC_DIR}/src"
    )
    if(TARGET luajit_build)
        add_dependencies(luajit luajit_build)
    endif()
endif()

message(status "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# Project executable
add_executable(${PROJECT_NAME} src/main.c)

# Link raylib and LuaJIT
target_include_directories(${PROJECT_NAME} PRIVATE
    ${raylib_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    "${LUAJIT_SRC_DIR}/src/luajit.lib"
    "${LUAJIT_SRC_DIR}/src/lua51.lib"
)

# Copy raylib DLL to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:raylib>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copying raylib DLL to output directory"
)

# Copy demo.lua to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/demo.lua"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/demo.lua"
    COMMENT "Copying demo.lua to output directory"
)