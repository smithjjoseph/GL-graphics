/****************
 * Title:   02_shaders/custom/main.cpp
 * Created: 2025/03/20
 * Author:  Joseph Smith
 ***************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;


// React to window resizing by setting viewport size to window size
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
    /****************
     * SETUP WINDOW
     ****************/

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
    // Set window resizing callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialise GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }


    // BUILD AND COMPILE SHADERS
    Shader customShader("../src/02_shaders/custom/shader.vert",
                        "../src/02_shaders/custom/shader.frag");

    /*************************************************
     * SETUP VERTICES, BUFFERS AND VERTEX ATTRIBUTES
     *************************************************/

    float vertices[] {
        // positions           // colors
        0.5f,  -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    // BR
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    // BL
        0.0f,   0.5f, 0.0f,    0.0f, 0.0f, 1.0f     // T
    };

    // Setup Vertex Buffer Object, Vertex Array Object, Element Buffer Object
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first to stop VBO code duplication
    glBindVertexArray(VAO);

    // Copy vertices array into the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link vertex attributes for position and colour attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    /***************
     * RENDER LOOP
     ***************/

    while(!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        customShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers, poll input
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocated no longer needed resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}