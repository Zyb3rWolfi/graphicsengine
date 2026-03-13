#ifndef GRAPHICSENGINE_LIGHT_H
#define GRAPHICSENGINE_LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 pos = glm::vec3(0.0f),
          glm::vec3 amb = glm::vec3(0.05f),
          glm::vec3 diff = glm::vec3(0.8f),
          glm::vec3 spec = glm::vec3(1.0f))
        : position(pos), ambient(amb), diffuse(diff), specular(spec) {}
};

#endif