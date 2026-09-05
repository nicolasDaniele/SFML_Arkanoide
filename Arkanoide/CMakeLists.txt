cmake_minimum_required(VERSION 3.28)

project(Arkanoide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ==================================================
# Sources
# ==================================================

set(SOURCES
    src/Ball.cpp
    src/Block.cpp
    src/Entity.cpp
    src/Game.cpp
    src/Label.cpp
    src/LevelManager.cpp
    src/MovableObject.cpp
    src/Paddle.cpp
    src/ScoreManager.cpp
    src/SoundManager.cpp
)


# ==================================================
# SFML
# ==================================================

if(EMSCRIPTEN)

    message(STATUS "Using SFML-EMCC")

    # Build SFML from source for Emscripten
    set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
    set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

    add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/SFML-EMCC"
        "${CMAKE_BINARY_DIR}/SFML"
        EXCLUDE_FROM_ALL
    )

else()

    message(STATUS "Using native SFML")

    # Precompiled native SFML
    set(SFML_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/SFML/lib/cmake/SFML"
        CACHE PATH "SFML CMake directory"
    )

    find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)

endif()


# ==================================================
# Executable
# ==================================================

add_executable(Arkanoide ${SOURCES})

target_include_directories(Arkanoide PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(Arkanoide PRIVATE
    SFML::Graphics
    SFML::Audio
)


# ==================================================
# Native assets & DLLs
# ==================================================

if(NOT EMSCRIPTEN)

    add_custom_command(TARGET Arkanoide POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
            "$<TARGET_FILE_DIR:Arkanoide>/Assets"
    )

    add_custom_command(TARGET Arkanoide POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics>
            $<TARGET_FILE:SFML::Audio>
            $<TARGET_FILE:SFML::Window>
            $<TARGET_FILE:SFML::System>
            "$<TARGET_FILE_DIR:Arkanoide>"
    )

endif()


# ==================================================
# Web / Emscripten options
# ==================================================

if(EMSCRIPTEN)

    message(STATUS "Building Arkanoide for Web")

    set_target_properties(Arkanoide PROPERTIES
        SUFFIX ".html"
    )

    target_link_options(Arkanoide PRIVATE
	"-sASYNCIFY=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES2=1"
        "-sMIN_WEBGL_VERSION=1"
        "-sMAX_WEBGL_VERSION=1"

	# shell.html config
	"--shell-file"
	"${CMAKE_CURRENT_SOURCE_DIR}/Shell/shell.html"

	# Assets loading
	"--preload-file"
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets@Assets"
    )

endif()