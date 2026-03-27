#include <glm/vec3.hpp>

#include "../Scene/Light.h"
//
// Created by Zyb3r on 13/02/2026.
//
using namespace glm;

#include "Renderer.h"

void Renderer::Draw(Scenemap &scene, Camera &camera, std::vector<Light> &lights) {
    for (auto &rootNode: scene.GetRootNodes()) {
        RenderNode(rootNode, glm::mat4(1.0f), camera, lights);

    }
}


void Renderer::RenderNode(const std::shared_ptr<SceneNode> &node, const glm::mat4 &parentTransform, Camera &camera,
                          std::vector<Light> &lights) {
    // 1 - First calculating the world transform for this node by combining the parent transform with the node's local transform
    // World = Parent * Local
    glm::mat4 transform = parentTransform * glm::translate(glm::mat4(1.0f), node->GetPosition());
    for (auto &meshNode: node->GetChildMeshes()) {
        // Skip if no shader is assigned to this mesh
        if (meshNode.shader == nullptr) {
            continue;
        }

        // Use the shader assigned to this mesh
        Shader *shader = meshNode.shader;
        shader->use();

        shader->setInt("numLights", static_cast<int>(lights.size()));

        for (int i = 0; i < static_cast<int>(lights.size()); i++) {
            std::string base = "lights[" + std::to_string(i) + "]";
            shader->setVec3(base + ".position", lights[i].position);
            shader->setVec3(base + ".ambient", lights[i].ambient);
            shader->setVec3(base + ".diffuse", lights[i].diffuse);
            shader->setVec3(base + ".specular", lights[i].specular);
            shader->setFloat(base + ".constant", lights[i].constant);
            shader->setFloat(base + ".linear", lights[i].linear);
            shader->setFloat(base + ".quadratic", lights[i].quadratic);
        }

        // Set camera-related uniforms
        shader->setVec3("viewPos", camera.position);
        shader->setVec3("objectColor", meshNode.mesh.objectColor);
        shader->setVec3("lightColor", vec3(1.0f));
        shader->setMat4("view", camera.GetViewMatrix());
        shader->setMat4("projection", camera.GetProjectionMatrix());

        // 2 - For each child mesh, we calculate the final transform by combining the node's world transform with the mesh's local transform
        glm::mat4 finalTransform = transform * meshNode.GetLocalTransform();

        // 3 - Set the final transform in the shader and draw the mesh
        shader->setMat4("model", finalTransform);

        shader->setVec3("material.ambient", meshNode.mesh.ambient);
        shader->setInt("material.diffuse", 0);
        shader->setFloat("material.shininess", meshNode.mesh.shininess);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(finalTransform)));
        shader->setMat3("normalMatrix", normalMatrix);

        if (meshNode.texture != nullptr) {
            shader->setBool("useTexture", true);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, meshNode.texture->texture); // Access your ID
            glActiveTexture(GL_TEXTURE1);
            if (meshNode.SpecularTexture != nullptr) {
                glBindTexture(GL_TEXTURE_2D, meshNode.SpecularTexture->texture); // Access your ID
                shader->setInt("material.specular", 1);
            } else {
                shader->setInt("material.specular", 0); // No specular map
            }

            glActiveTexture(GL_TEXTURE2);
            if (meshNode.EmissionTexture != nullptr) {
                glBindTexture(GL_TEXTURE_2D, meshNode.EmissionTexture->texture); // Access your ID
                shader->setInt("material.emission", 2);
            } else {
                shader->setInt("material.emission", 10); // No emission map
            }

            glActiveTexture(GL_TEXTURE3);
            if (meshNode.NormalTexture != nullptr) {
                glBindTexture(GL_TEXTURE_2D, meshNode.NormalTexture->texture);
                shader->setInt("normalMap", 3);
                shader->setBool("useNormalMap", true);
            } else {
                shader->setBool("useNormalMap", false);
            }

            shader->setInt("ourTexture", 0);
        } else {
            shader->setBool("useTexture", false);
        }

        meshNode.mesh.Draw();
    }

    for (auto &childNode: node->GetChildNodes()) {
        // 4 - Recursively render child nodes, passing the current node's world transform as the new parent transform
        RenderNode(childNode, transform, camera, lights);
    }
}
