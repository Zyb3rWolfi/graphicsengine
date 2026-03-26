//
// Created by Zyb3r on 31/01/2026.
//

#ifndef GRAPHICSENGINE_SHADER_H
#define GRAPHICSENGINE_SHADER_H

#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    unsigned int Compile(const char *Code, GLenum type);

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setMat3(const std::string &name, const glm::mat3 &mat) const;

private:
    std::string Get(const char *path);
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //GRAPHICSENGINE_SHADER_H
