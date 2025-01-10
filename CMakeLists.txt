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

set(GLAD-SRC src/glad.c)

set(GL-GRAPHICS-SRC
    src/triangles.cpp
    src/shaders.cpp
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
    get_filename_component(filename ${source} NAME_WE)
    message(STATUS "Adding executable for ${source}")
    add_executable(${filename} WIN32 ${source} ${GLAD-SRC})
    target_link_libraries(${filename} ${OPENGL_LIBRARIES} glfw)
endforeach()

# Sets the default startup peoject for a Visual Studio solution (.sln)
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GL-Graphics' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GL-Graphics)
    endif()
endif()
