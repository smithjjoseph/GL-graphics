/****************
 * Title:   main.cpp
 * Created: 2025/01/06
 * Author:  Joseph Smith
 ***************/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;


// React to window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


// Query keypresses and react to result
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(void)
{
    // Initialise/configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__    // MAC OS X only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(
        WIN_WIDTH, WIN_HEIGHT, "OpenGLGraphics", NULL, NULL
    );
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Set OpenGL viewport and window resizing callback
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers, poll input
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
