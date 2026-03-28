//
// Created by Zyb3r on 13/02/2026.
//

#include "Scenemap.h"
#include <glm/gtc/matrix_transform.hpp>

// ============================================================================
// MeshNode Implementation
// ============================================================================

// Update the constructor
MeshNode::MeshNode(Mesh&& m, Texture* t,Texture* st,Texture* et,Texture* nt, Shader* s, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
    : texture(t), EmissionTexture(et), NormalTexture(nt), mesh(std::move(m)), shader(s), SpecularTexture(st), localPosition(pos), localRotation(rot), localScale(scale) {}

// Getting the local transform for the mesh node
glm::mat4 MeshNode::GetLocalTransform() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, localPosition);
    transform = glm::rotate(transform, glm::radians(localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, localScale);
    return transform;
}

// ============================================================================
// SceneNode Implementation
// ============================================================================

SceneNode::SceneNode(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
    : worldPosition(pos), worldRotation(rot), worldScale(scale) {}

// Getting the world transform for the scene node
glm::mat4 SceneNode::GetWorldTransform() const {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, worldPosition);
    transform = glm::rotate(transform, glm::radians(worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, worldScale);
    return transform;
}

// Adding a child mesh with local transform
void SceneNode::AddChildMesh(Mesh&& mesh, Texture* texture,Texture* specularTexture,Texture* EmissionTexture, Texture* NormalTexture, Shader* shader, glm::vec3 localPos, glm::vec3 localRot, glm::vec3 localScale) {
    childMeshes.emplace_back(std::move(mesh), texture,specularTexture,EmissionTexture,NormalTexture, shader, localPos, localRot, localScale);
}

void SceneNode::Delete(const std::shared_ptr<SceneNode>& node, Scenemap& scenemap) {
    if (node->parent) {
        node->parent->RemoveChildNode(node);
    } else {
        // If it's a root node, we need to find and remove it from the scenemap
        // This requires access to the scenemap, which is not ideal. Consider refactoring for better design.
        scenemap.RemoveRootNode(node);

    }
}

// Adding a child node to the scene node
void SceneNode::AddChildNode(const std::shared_ptr<SceneNode>& node) {
    node->parent = this; // Set parent reference in the child node
    childNodes.push_back(node);
}


// Getting child meshes
std::vector<MeshNode>& SceneNode::GetChildMeshes() {
    return childMeshes;
}

// Getting child nodes
std::vector<std::shared_ptr<SceneNode>>& SceneNode::GetChildNodes() {
    return childNodes;
}

// Removing a child mesh at a specific index
void SceneNode::RemoveChildMesh(size_t index) {
    if (index < childMeshes.size()) {
        childMeshes.erase(childMeshes.begin() + static_cast<std::vector<MeshNode>::difference_type>(index));
    }
}


void SceneNode::SetPosition(glm::vec3 pos) {
    worldPosition = pos;
}

void SceneNode::SetRotation(glm::vec3 rot) {
    worldRotation = rot;
}

void SceneNode::SetScale(glm::vec3 scale) {
    worldScale = scale;
}

glm::vec3 &SceneNode::GetPosition() {
    return worldPosition;
}

glm::vec3 &SceneNode::GetRotation() {
    return worldRotation;
}

glm::vec3 &SceneNode::GetScale() {
    return worldScale;
}

void SceneNode::Translate(glm::vec3 offset) {
    worldPosition += offset;
}

void SceneNode::Rotate(glm::vec3 rotation) {
    worldRotation += rotation;
}

void SceneNode::Scale(glm::vec3 scale) {
    worldScale *= scale;
}

void SceneNode::RemoveChildNode(std::shared_ptr<SceneNode> node) {
    auto it = std::find(childNodes.begin(), childNodes.end(), node);
    if (it != childNodes.end()) {
        childNodes.erase(it);
        node->parent = nullptr; // Optional: Clear parent reference in the child node
    }

}


// ============================================================================
// Scenemap Implementation
// ============================================================================

std::shared_ptr<SceneNode> Scenemap::AddRootNode(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    auto node = std::make_shared<SceneNode>(pos, rot, scale);
    rootNodes.push_back(node);
    return node;
}

std::vector<std::shared_ptr<SceneNode>>& Scenemap::GetRootNodes() {
    return rootNodes;
}

void Scenemap::RemoveRootNode(std::shared_ptr<SceneNode> node) {
    // Standard C++ idiom to remove a specific element from a vector
    rootNodes.erase(std::remove(rootNodes.begin(), rootNodes.end(), node), rootNodes.end());}

void Scenemap::Clear() {
    rootNodes.clear();
}

size_t Scenemap::GetRootNodeCount() const {
    return rootNodes.size();
}
