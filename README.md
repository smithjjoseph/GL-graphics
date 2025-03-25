# GL-graphics

This project uses CMake and MinGW to build a project with [GLFW](https://www.glfw.org/)/[GLAD](https://github.com/Dav1dde/glad) for use with the [OpenGL](https://www.opengl.org/) specification. This project is designed to be multi-platform and runs on most C++ build systems including Windows, Linux and MacOS.

This README acts as a source of information for this project, an instructional for building this project as well as a journal of my learnings over the course of this project.

Much of this project will follow alongside the extremely helpful teachings at https://learnopengl.com.

## CMake

### GLFW-CMake-Starter

This project used a template called [GLFW-CMake-Starter](https://github.com/juliettef/GLFW-CMake-starter). Once cloned, this template can be used to generate the makefiles for GLFW from the GLFW git submodule. This template has been used for its multi-platform compatability.

### Modifications

- Add warning flags to compilation of each project source file
- Change to create an executable for each project source file
- Modified to use newly generated version of GLAD [(continued)](#glad)

### Using CMake to build the project

From a command prompt in the project top-level directory:
1. `mkdir build`
1. `cd build`
1. `cmake ..` (for MinGW the build system generator needs to be specified using the -G option: `cmake .. -G "MinGW Makefiles"`)
1. Either run `make all` or for Visual Studio open `GLFW-CMake-starter.sln` or for MinGW run `mingw32-make`

## GLAD

GLAD is used to load and generate OS-dependant functions for use in various standards in this case OpenGL.

The aforementioned GLFW CMake starter included a section intended for giving the ability to use the version of GLAD2 (header only) packaged with the GLFW submodule.

```cmake
set(GLAD_GL "")

...

option(GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON)

if(GLFW-CMAKE-STARTER-USE-GLFW-GLAD)
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h")
endif()

...

add_executable(GLFW-CMake-starter WIN32 ${GLFW-CMAKE-STARTER-SRC} ${GLAD_GL})
```
This would then be used with:
```C++
/* Since the header only implementation is being used glad needs a guard
   https://github.com/Dav1dde/glad/wiki/C#header-only */
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
```

However, since most resources use GLAD1, this will be removed and ignored. Instead I will use the full version of GLAD1 obtained from https://glad.dav1d.de/. (Using settings: C/C++, OpenGL, gl V4.6 Core, Generate a Loader)
