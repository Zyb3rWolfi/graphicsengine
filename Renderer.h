//
// Created by Zyb3r on 13/02/2026.
//

#ifndef GRAPHICSENGINE_RENDERER_H
#define GRAPHICSENGINE_RENDERER_H
#include "Camera.h"
#include "Scenemap.h"
#include "Shader.h"


class Renderer {
public:
    void Draw(Scenemap& scene, Camera& camera, glm::vec3 lightPos);
private:
    // Recursive function to handle the parent-child math
    void RenderNode(const std::shared_ptr<SceneNode>& node, const glm::mat4& parentTransform, Camera& camera, glm::vec3 lightPos);
};


#endif //GRAPHICSENGINE_RENDERER_H