/****************
 * Title:   02_shaders/question/main.cpp
 * Created: 2025/01/10
 * Author:  Joseph Smith
 ***************/

#include <iostream>

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VAR_NAME(var) (#var)

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

// Vertex shader GLSL
const char* vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
const char* FragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 chosenColor;\n"
    "void main() {\n"
    "    FragColor = vec4(chosenColor);\n"
    "}\0";


// React to window resizing by setting viewport size to window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


// Query keypresses and react to result
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


/**
 * Reports on the status of the compilation of a GL shader
 *
 * @param shader      shader object to be queried for compilation status
 * @param identifier  shader variable name to reference in log
 */
void checkShaderCompilation(GLuint shader, const char* identifier) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << identifier <<
            "::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "SUCCESS::SHADER::" << identifier <<
            "::COMPILATION_SUCCESS" << std::endl;
    }
}


/**
 * Reports on the status of the linkage of a GL program
 *
 * @param shader      program object to be queried for linkage status
 * @param identifier  shader variable name to reference in log
 */
void checkProgramLinkage(GLuint program, const char* identifier) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::" << identifier <<
            "::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "SUCCESS::PROGRAM::" << identifier <<
            "::COMPILATION_SUCCESS" << std::endl;
    }
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


    /*******************
     * COMPILE SHADERS
     *******************/

    // Setup vertex shader and compile
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader, VAR_NAME(vertexShader));

    // Setup fragment shader and compile
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    checkShaderCompilation(FragmentShader, VAR_NAME(FragmentShader));

    // Setup shader program
    unsigned int ShaderProgram;
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    checkProgramLinkage(ShaderProgram, VAR_NAME(ShaderProgram));

    // Delete unused shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(FragmentShader);


    /*************************************************
     * SETUP VERTICES, BUFFERS AND VERTEX ATTRIBUTES
     *************************************************/

    float vertices[] {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);


    /***************
     * RENDER LOOP
     ***************/

    while(!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderProgram);

        // Set uniform (mario kart [?] box effect)
        float timeValue = glfwGetTime();
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(ShaderProgram,
            "chosenColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, 0.5f, 1.0f);

        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers, poll input
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocated no longer needed resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

    glfwTerminate();
    return 0;
}