//
// Created by Zyb3r on 30/01/2026.
//

#ifndef GRAPHICSENGINE_VERTEX_H
#define GRAPHICSENGINE_VERTEX_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 Position;  // 3 floats (offset 0)
    glm::vec3 Color;     // 3 floats (offset 3)
    glm::vec2 TexCoords; // 2 floats (offset 6)
    glm::vec3 Normal;    // 3 floats (offset 8)
    glm::vec3 Tangent;   // 3 floats (offset 11) - Total: 14 floats per vertex (56 bytes)
};

#endif //GRAPHICSENGINE_VERTEX_H
