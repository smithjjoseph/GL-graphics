#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // Contains program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath) {
        // RETRIEVE VERTEX/FRAGMENT SOURCE FROM PATH
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // Ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_UNSUCCESFULLY_READ\n" << 
                e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // COMPILE AND LINK SHADERS
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Report on status of vertex shader compilation
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << 
                infoLog << std::endl;
        } else {
            std::cout << "SUCCESS::SHADER::VERTEX::COMPILATION_SUCCESS" << 
                std::endl;
        }
        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Report on status of fragment shader compilation
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << 
                infoLog << std::endl;
        } else {
            std::cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION_SUCCESS" << 
                std::endl;
        }
        // Create shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // Report on status of shader program linkage
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << 
                infoLog << std::endl;
        } else {
            std::cout << "SUCCESS::SHADER::PROGRAM::LINKAGE_SUCCESS" << 
                std::endl;
        }
        // Delete unused shader objects
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };

    // Activate the shader
    void use() {
        glUseProgram(ID);
    }
    // Utility uniform functions
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    /**
     * Reports on the status of the compilation of a GL shader
     * 
     * @param shader      shader object to be queried for compilation status
     * @param identifier  shader variable name to reference in log
     */
    void checkCompilation(GLuint shader, const char* identifier) {
        int success;
        char infoLog[1024];
        if (identifier != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << identifier <<
                    "::COMPILATION_FAILED\n" << infoLog << std::endl;
            } else {
                std::cout << "SUCCESS::SHADER::" << identifier <<
                    "::COMPILATION_SUCCESS" << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << 
                    infoLog << std::endl;
            } else {
                std::cout << "SUCCESS::SHADER::PROGRAM::LINKAGE_SUCCESS" << 
                    std::endl;
            }
        }
    }
};

#endif