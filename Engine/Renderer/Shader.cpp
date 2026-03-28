//
// Created by Zyb3r on 31/01/2026.
//

#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Helper.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Use your new Get() helper for both
    std::string vCodeStr = Get(vertexPath);
    std::string fCodeStr = Get(fragmentPath);

    // Compile both using your new Compile() helper
    unsigned int vertex = Compile(vCodeStr.c_str(), GL_VERTEX_SHADER);
    unsigned int fragment = Compile(fCodeStr.c_str(), GL_FRAGMENT_SHADER);

    // Link into ONE Program ID
    this->ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Error checking for the linked program
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        Helper::Log("Shader linking failed for program ID: " + std::to_string(ID) + "\nError Log: " + infoLog, LogLevel::ERROR);
    }

    // Clean up individual stages
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Compiles a shader of a given type (vertex or fragment) from source code
unsigned int Shader::Compile(const char *Code, GLenum type) {

    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &Code, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::cout << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        Helper::Log("Shader compilation failed for " + typeStr + " shader.\nError Log: " + infoLog, LogLevel::ERROR);
    }
    return shader;
}

// Open the given shader program for use (make it the active shader)

std::string Shader::Get(const char *path) {
    std::string ShaderCode;
    std::ifstream ShaderFile;

    std::cout << "Attempting to load shader from: " << path << std::endl;
    Helper::Log("Loading shader from: " + std::string(path), LogLevel::INFO);

    ShaderFile.exceptions(std::ifstream::badbit);
    try {
        // opening file
        ShaderFile.open(path);
        std::stringstream vShaderStream;
        // read files buffer content into streams
        vShaderStream << ShaderFile.rdbuf();
        // close file handlers
        ShaderFile.close();
        // convert stream into string
        ShaderCode = vShaderStream.str();

        std::cout << "Successfully loaded shader, size: " << ShaderCode.length() << " bytes" << std::endl;
        Helper::Log("Successfully loaded shader: " + std::string(path) + ", size: " + std::to_string(ShaderCode.length()) + " bytes", LogLevel::INFO);
        return ShaderCode;

    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
        Helper::Log("Failed to read shader file: " + std::string(path) + "\nException: " + e.what(), LogLevel::ERROR);
    }
    return "";
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    // 1. Find the location of the uniform in the shader
    // 2. 1 = sending one matrix
    // 3. GL_FALSE = do not transpose (GLM matrices are already column-major)
    // 4. glm::value_ptr = gets the raw float array from the GLM matrix object
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}
