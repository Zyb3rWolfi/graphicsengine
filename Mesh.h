//
// Created by Zyb3r on 06/02/2026.
//

#ifndef GRAPHICSENGINE_MESH_H
#define GRAPHICSENGINE_MESH_H
#include <glm/fwd.hpp>

#include "Shader.h"


class Mesh {
    public:
    glm::vec3 objectColor;
    glm::vec3 lightColor;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    unsigned int vbo, vao, ebo;
    void SetLighting(glm::vec3 lightColor, glm::vec3 objectColor, Shader shader);
    int vertexCount;
    Mesh(float vertices[], unsigned int indices[], unsigned int vertexSize, unsigned int indicesSize);

    Mesh(float * arr, unsigned int * indices);

    void Draw();
    void SetupAttributes();
};


#endif //GRAPHICSENGINE_MESH_H