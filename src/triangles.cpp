/****************
 * Title:   triangles.cpp
 * Created: 2025/01/06
 * Author:  Joseph Smith
 ***************/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VAR_NAME(var) (#var)

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
// Vertex shader GLSL
const char* vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* orangeFragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\0";
const char* yellowFragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(0.5, 0.5, 0.1, 1.0);\n"
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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Toggles between filled and lined polygons
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        polygonMode[0] = (polygonMode[0] == GL_LINE) ? GL_FILL : GL_LINE;
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
    }
}


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


void checkProgramLinkage(GLuint program, const char* identifier) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::" << identifier << 
            "::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "SUCCESS::SHADER::" << identifier << 
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

    // Setup orange fragment shader and compile
    unsigned int orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
    glCompileShader(orangeFragmentShader);
    checkShaderCompilation(orangeFragmentShader, VAR_NAME(orangeFragmentShader));
    // Setup yellow fragment shader and compile
    unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
    glCompileShader(yellowFragmentShader);
    checkShaderCompilation(yellowFragmentShader, VAR_NAME(yellowFragmentShader));

    // Setup orange shader program
    unsigned int orangeShaderProgram;
    orangeShaderProgram = glCreateProgram();
    glAttachShader(orangeShaderProgram, vertexShader);
    glAttachShader(orangeShaderProgram, orangeFragmentShader);
    glLinkProgram(orangeShaderProgram);
    // Check Program linkage success
    checkProgramLinkage(orangeShaderProgram, VAR_NAME(orangeShaderProgram));

    // Setup yellow shader program
    unsigned int yellowShaderProgram;
    yellowShaderProgram = glCreateProgram();
    glAttachShader(yellowShaderProgram, vertexShader);
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);
    // Check Program linkage success
    checkProgramLinkage(yellowShaderProgram, VAR_NAME(yellowShaderProgram));

    // Delete unused shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragmentShader);
    glDeleteShader(yellowFragmentShader);

    /*************************************************
     * SETUP VERTICES, BUFFERS AND VERTEX ATTRIBUTES
     *************************************************/

    float vertices[] {
        -0.8f,  0.5f, 0.0f,
        -0.8f, -0.5f, 0.0f,
        -0.2f, -0.5f, 0.0f,
        0.8f,  0.5f, 0.0f,
        0.8f, -0.5f, 0.0f,
        0.2f, -0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    // Setup Vertex Buffer Object (VBO) and Vertex Array Object (VAO)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO first to stop VBO/EBO code duplication
    glBindVertexArray(VAO);

    // Copy vertices array into the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Copy indices array into the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up keypress callback
    glfwSetKeyCallback(window, key_callback);

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
        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers, poll input
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocated no longer needed resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(orangeShaderProgram);
    glDeleteProgram(yellowShaderProgram);

    glfwTerminate();
    return 0;
}
