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

Mesh::Mesh(float vertices[], unsigned int indices[], unsigned int vertexSize, unsigned int indicesSize) {
    vertexCount = indicesSize / sizeof(unsigned int);
    objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    SetupAttributes();
}

void Mesh::SetupAttributes() {
    GLsizei stride = sizeof(Vertex);

    // Location 0: Position (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Position));

    // Location 1: Color (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Color));

    // Location 2: Texture Coordinates (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, TexCoords));

    // Location 3: Normals (vec3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Normal));

    // Location 3: Normals (vec3)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Tangent));




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
