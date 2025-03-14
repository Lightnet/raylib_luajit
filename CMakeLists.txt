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

# Add raylib 5.5 (shared for FFI)
CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG 5.5
    OPTIONS
        "BUILD_EXAMPLES OFF"
        "BUILD_GAMES OFF"
        "BUILD_SHARED_LIBS ON"
)

# Add raygui 4.0 (header-only)
CPMAddPackage(
    NAME raygui
    GITHUB_REPOSITORY raysan5/raygui
    GIT_TAG 4.0
    DOWNLOAD_ONLY YES
)

# Add enet 2.6.2 (header-only, static linking)
CPMAddPackage(
    NAME enet
    GITHUB_REPOSITORY zpl-c/enet
    GIT_TAG v2.6.2
    DOWNLOAD_ONLY YES
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

# Project executable with enet statically linked
add_executable(${PROJECT_NAME} 
  src/main.c 
  src/enet-luajit.c 
  src/raylib-luajit.c
)

# Link raylib, LuaJIT, and include raygui and enet
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${raylib_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
    ${raygui_SOURCE_DIR}/src
    ${enet_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    "${LUAJIT_SRC_DIR}/src/luajit.lib"
    "${LUAJIT_SRC_DIR}/src/lua51.lib"
)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)
endif()

# Copy raylib.dll to output directory
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