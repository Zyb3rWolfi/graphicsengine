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
    Shader(const char* vertexPAth, const char* fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //GRAPHICSENGINE_SHADER_H