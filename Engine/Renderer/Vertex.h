//
// Created by Zyb3r on 30/01/2026.
//

#ifndef GRAPHICSENGINE_VERTEX_H
#define GRAPHICSENGINE_VERTEX_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#endif //GRAPHICSENGINE_VERTEX_H

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    // You even have Tangents in your shader already!
    glm::vec3 Tangent;
};