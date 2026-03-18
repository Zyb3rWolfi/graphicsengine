//
// Created by Zyb3r on 06/02/2026.
//

#include "Mesh.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb_image.h"

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

    glBindBuffer(GL_ARRAY_BUFFER, lightVAO);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    SetupAttributes();
}

void Mesh::SetupAttributes() {
    int stride = 11 * sizeof(float);
    // 1: first location
    // 2: tells the size of our data is 3
    // 3: The data type is a float
    // 4: Tells how many steps we need to jump to the next data
    // 5: where does our data start?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(float) * 6));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void *) (sizeof(float) * 8));
    glEnableVertexAttribArray(3);
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
