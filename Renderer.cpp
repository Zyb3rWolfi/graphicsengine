//
// Created by Zyb3r on 13/02/2026.
//

#include "Renderer.h"

void Renderer::Draw(Scenemap& scene, Shader& shader, Camera& camera) {
    shader.use();// Use the shader program

    shader.setMat4("view", camera.GetViewMatrix());

    for (auto& rootNode : scene.GetRootNodes()) {
        RenderNode(rootNode, glm::mat4(1.0f), shader);
    }
}


void Renderer::RenderNode(const std::shared_ptr<SceneNode> &node, const glm::mat4 &parentTransform, Shader &shader) {

    // 1 - First calculating the world transform for this node by combining the parent transform with the node's local transform
    // World = Parent * Local
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), node->GetPosition());

    for (auto& meshNode : node->GetChildMeshes()) {
        // 2 - For each child mesh, we calculate the final transform by combining the node's world transform with the mesh's local transform
        glm::mat4 finalTransform = transform * meshNode.GetLocalTransform();

        // 3 - Set the final transform in the shader and draw the mesh
        shader.setMat4("model", finalTransform);
        meshNode.mesh->Draw();
    }

    for (auto& childNode : node->GetChildNodes()) {
        // 4 - Recursively render child nodes, passing the current node's world transform as the new parent transform
        RenderNode(childNode, transform, shader);
    }
}
