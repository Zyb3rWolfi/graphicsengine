//
// Created by Zyb3r on 06/02/2026.
//

#include "Mesh.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb_image.h"
#include "Vertex.h"

glm::vec3 objectColor;
glm::vec3 lightColor;

Mesh::Mesh(float vertices[], unsigned int indices[], unsigned int vertexSize, unsigned int indicesSize)
    : vbo(0), vao(0), ebo(0), vertexCount(0) {
    vertexCount = indicesSize / sizeof(unsigned int);
    objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    ambient = glm::vec3(0.1f);
    diffuse = glm::vec3(0.5f);
    specular = glm::vec3(1.0f);
    shininess = 32.0f;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    SetupAttributes();
}

Mesh::~Mesh() {
    if (vao != 0) glDeleteVertexArrays(1, &vao);
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (ebo != 0) glDeleteBuffers(1, &ebo);
}

Mesh::Mesh(Mesh&& other) noexcept
    : objectColor(other.objectColor), lightColor(other.lightColor),
      ambient(other.ambient), diffuse(other.diffuse), specular(other.specular),
      shininess(other.shininess), vbo(other.vbo), vao(other.vao), ebo(other.ebo),
      vertexCount(other.vertexCount) {
    other.vbo = 0;
    other.vao = 0;
    other.ebo = 0;
    other.vertexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        // Clean up existing resources
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        if (ebo != 0) glDeleteBuffers(1, &ebo);

        // Take ownership of other's resources
        vbo = other.vbo;
        vao = other.vao;
        ebo = other.ebo;
        vertexCount = other.vertexCount;
        objectColor = other.objectColor;
        lightColor = other.lightColor;
        ambient = other.ambient;
        diffuse = other.diffuse;
        specular = other.specular;
        shininess = other.shininess;

        // Invalidate other
        other.vbo = 0;
        other.vao = 0;
        other.ebo = 0;
        other.vertexCount = 0;
    }
    return *this;
}

void Mesh::SetupAttributes() {
    std::cout << "Size of Vertex struct: " << sizeof(Vertex) << " bytes" << std::endl;
    GLsizei stride = 14 * sizeof(float);  // 56 bytes

    // Location 0: Position (vec3) - offset 0 bytes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));

    // Location 1: Color (vec3) - offset 12 bytes (3 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    // Location 2: Texture Coordinates (vec2) - offset 24 bytes (6 floats)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    // Location 3: Normals (vec3) - offset 32 bytes (8 floats)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));

    // Location 4: Tangent (vec3) - offset 44 bytes (11 floats)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11 * sizeof(float)));
}

void Mesh::Draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::SetLighting(glm::vec3 lightColor, glm::vec3 objectColor, Shader shader) {
    shader.use();
    shader.setVec3("objectColor", objectColor);
    shader.setVec3("lightColor", lightColor);
}
