#ifndef GRAPHICSENGINE_LIGHT_H
#define GRAPHICSENGINE_LIGHT_H

#include <glm/glm.hpp>
enum class LightType {
    DIRECTIONAL = 0,
    POINT = 1,
    SPOT = 2
};

struct Light {
    LightType LightType;
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(17.5f));

    Light(glm::vec3 pos = glm::vec3(0.0f),
          glm::vec3 amb = glm::vec3(0.05f),
          glm::vec3 diff = glm::vec3(0.8f),
          glm::vec3 spec = glm::vec3(1.0f),
          float c = 1.0f, float l = 0.09f, float q = 0.032f) // Default values for ~50 unit range
        : position(pos), ambient(amb), diffuse(diff), specular(spec),
          constant(c), linear(l), quadratic(q) {}
};

#endif