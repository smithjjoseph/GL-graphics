# Modified from https://github.com/enkisoftware/GLFW-CMake-starter
# Copyright (c) 2021 Juliette Foucaut and Doug Binks

cmake_minimum_required(VERSION 3.10)
project(GL-Graphics)

# Find system OpenGL and include directories
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} include)

# Configure and add GLFW subdirectory
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(glfw)

# Hides console while still allowing for int main() as the application entry point
if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

# Setup for each executable
set(GLAD-SRC src/glad.c)

set(TRIANGLES-SRC src/01_triangle/triangles/main.cpp)

set(SHADERS-QUESTION-SRC src/02_shaders/question/main.cpp)

set(SHADERS-RAINBOW-SRC src/02_shaders/rainbow/main.cpp)

set(SHADERS-CUSTOM-SRC
    src/02_shaders/custom/main.cpp
    src/02_shaders/custom/shader.hpp
    src/02_shaders/custom/shader.vert
    src/02_shaders/custom/shader.frag
)

set(GL-GRAPHICS-SRC
    TRIANGLES-SRC
    SHADERS-QUESTION-SRC
    SHADERS-RAINBOW-SRC
    SHADERS-CUSTOM-SRC
    TEXTURES-BASIC-SRC
)

# Add warnings to compilation (Add /WX for MSVC or -Werror for other to fail on error)
if(MSVC)
    set(COMPILE-FLAGS "/W4 /wd4189")
else()
    set(COMPILE-FLAGS "-Wall -Wextra -Wpedantic -Wno-unused")
endif()
# Only checks project files
foreach(source ${GL-GRAPHICS-SRC})
    message(STATUS "Setting compile flags for file: ${source}")
    set_source_files_properties(${source} PROPERTIES COMPILE_FLAGS ${COMPILE-FLAGS})
endforeach()

# Create an executable for each source also link libraries
foreach(source ${GL-GRAPHICS-SRC})
    # Get list of sources
    set(source-list ${${source}})
    # Create name of executable
    list(GET source-list 0 main-file)
    get_filename_component(filename ${main-file} DIRECTORY)
    string(REPLACE "src/" "" filename ${filename})
    string(REPLACE "/" "_" filename ${filename})
    message("[+] EXECUTABLE for ${source}")
    message("    FILENAME: ${filename}")
    message("    SOURCES: ${source-list}")

    add_executable(${filename} WIN32 ${source-list} ${GLAD-SRC})
    target_link_libraries(${filename} ${OPENGL_LIBRARIES} glfw)
endforeach()

# Sets the default startup project for a Visual Studio solution (.sln)
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GL-Graphics' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GL-Graphics)
    endif()
endif()
