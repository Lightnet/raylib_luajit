cmake_minimum_required(VERSION 3.14)
project(raylib_luajit LANGUAGES C CXX)

# Set up cache directory for built artifacts
set(CACHE_DIR "${CMAKE_BINARY_DIR}/cache")
file(MAKE_DIRECTORY ${CACHE_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
    file(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake"
        "${CMAKE_BINARY_DIR}/cmake/CPM.cmake"
    )
endif()
include(${CMAKE_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME raylib
    GITHUB_REPOSITORY raysan5/raylib
    GIT_TAG 5.5
    OPTIONS
        "BUILD_EXAMPLES OFF"
        "BUILD_GAMES OFF"
        "BUILD_SHARED_LIBS ON"
)

CPMAddPackage(
    NAME raygui
    GITHUB_REPOSITORY raysan5/raygui
    GIT_TAG 4.0
    DOWNLOAD_ONLY YES
)

CPMAddPackage(
    NAME enet
    GITHUB_REPOSITORY zpl-c/enet
    GIT_TAG v2.6.2
    DOWNLOAD_ONLY YES
)

CPMAddPackage(
    NAME luajit
    GITHUB_REPOSITORY openresty/luajit2
    GIT_TAG v2.1-20250117
    DOWNLOAD_ONLY YES
)

CPMAddPackage(
    NAME flecs
    GITHUB_REPOSITORY SanderMertens/flecs
    GIT_TAG v4.0.4
    OPTIONS
        "FLECS_STATIC OFF"
        "FLECS_SHARED ON"
)

if(luajit_ADDED)
    set(LUAJIT_SRC_DIR ${luajit_SOURCE_DIR})
    set(LUAJIT_DLL_SRC "${LUAJIT_SRC_DIR}/src/lua51.dll")
    set(LUAJIT_LIB_SRC "${LUAJIT_SRC_DIR}/src/lua51.lib")
    set(LUAJIT_DLL "${CACHE_DIR}/lua51.dll")
    set(LUAJIT_LIB "${CACHE_DIR}/lua51.lib")

    # Only rebuild LuaJIT if cached files are missing or source changed
    if(NOT EXISTS ${LUAJIT_DLL} OR NOT EXISTS ${LUAJIT_LIB} OR "${LUAJIT_SRC_DIR}/src/luajit.c" IS_NEWER_THAN ${LUAJIT_DLL})
        set(VS_VCVARSALL "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat")
        add_custom_command(
            OUTPUT ${LUAJIT_DLL_SRC} ${LUAJIT_LIB_SRC}
            COMMAND cmd /c "call \"${VS_VCVARSALL}\" x64 && cd /d ${LUAJIT_SRC_DIR}/src && call ${LUAJIT_SRC_DIR}/src/msvcbuild.bat dll"
            DEPENDS ${LUAJIT_SRC_DIR}/src/luajit.c
            WORKING_DIRECTORY ${LUAJIT_SRC_DIR}/src
            COMMENT "Building LuaJIT shared library with MSVC"
        )
        # Copy to cache after building
        add_custom_command(
            OUTPUT ${LUAJIT_DLL} ${LUAJIT_LIB}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUAJIT_DLL_SRC} ${LUAJIT_DLL}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUAJIT_LIB_SRC} ${LUAJIT_LIB}
            DEPENDS ${LUAJIT_DLL_SRC} ${LUAJIT_LIB_SRC}
            COMMENT "Caching LuaJIT build artifacts"
        )
        add_custom_target(luajit_build DEPENDS ${LUAJIT_DLL} ${LUAJIT_LIB})
    endif()

    add_library(luajit SHARED IMPORTED)
    set_target_properties(luajit PROPERTIES
        IMPORTED_LOCATION "${LUAJIT_DLL}"
        IMPORTED_IMPLIB "${LUAJIT_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${LUAJIT_SRC_DIR}/src"
    )
    if(TARGET luajit_build)
        add_dependencies(luajit luajit_build)
    endif()
endif()

add_executable(${PROJECT_NAME} 
    src/main.c 
    src/enet_luajit.c 
    src/raylib_luajit.c 
    src/raymath_luajit.c 
    src/raygui_luajit.c
    src/flecs_luajit.c
)

if(TARGET luajit_build)
    add_dependencies(${PROJECT_NAME} luajit_build)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${raylib_SOURCE_DIR}/src
    ${luajit_SOURCE_DIR}/src
    ${raygui_SOURCE_DIR}/src
    ${enet_SOURCE_DIR}/include
    ${flecs_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    flecs
    luajit
)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)
endif()

# Copy DLLs to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:raylib>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/raylib.dll"
    COMMENT "Copying raylib.dll to output directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LUAJIT_DLL}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/lua51.dll"
    COMMENT "Copying lua51.dll to output directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${flecs_BINARY_DIR}/Debug/flecs.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/flecs.dll"
    COMMENT "Copying flecs.dll to output directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/demo.lua"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/demo.lua"
    COMMENT "Copying demo.lua to output directory"
)