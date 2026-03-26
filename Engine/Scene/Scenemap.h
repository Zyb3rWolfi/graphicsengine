//
// Created by Zyb3r on 13/02/2026.
//

#ifndef GRAPHICSENGINE_SCENEMAP_H
#define GRAPHICSENGINE_SCENEMAP_H
#include "../Renderer/Mesh.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"

// Forward declarations
class SceneNode;

// Represents a mesh with its local transform relative to parent
struct MeshNode {
    Texture* texture;                       // Texture for the mesh
    Texture* EmissionTexture;                // Emission map for the mesh (optional)
    Texture* NormalTexture;                // Normal map for the mesh (optional)
    Mesh* mesh;                          // Pointer to the mesh
    Shader* shader;
    Texture* SpecularTexture;                // Specular map for the mesh (optional)
    glm::vec3 localPosition;             // Position relative to parent
    glm::vec3 localRotation;             // Rotation in degrees (x, y, z)
    glm::vec3 localScale;                // Scale relative to parent

    MeshNode(Mesh* m, Texture* t = nullptr,Texture* st = nullptr,Texture* et = nullptr,Texture* nt = nullptr, Shader* s = nullptr, glm::vec3 pos = glm::vec3(0.0f),
             glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    // Get the local transform matrix
    [[nodiscard]] glm::mat4 GetLocalTransform() const;
};

// Scene node with world space transform and child meshes
class SceneNode {
private:
    glm::vec3 worldPosition;
    glm::vec3 worldRotation;             // Rotation in degrees
    glm::vec3 worldScale;
    std::vector<MeshNode> childMeshes;   // Child meshes with local transforms
    std::vector<std::shared_ptr<SceneNode>> childNodes;  // Child nodes for hierarchy

public:
    SceneNode(glm::vec3 pos = glm::vec3(0.0f),
              glm::vec3 rot = glm::vec3(0.0f),
              glm::vec3 scale = glm::vec3(1.0f));

    // Get the world transform matrix of this node
    [[nodiscard]] glm::mat4 GetWorldTransform() const;

    // Add a child mesh with local transform
    void AddChildMesh(Mesh* mesh, Texture* texture = nullptr,Texture* specularTexture = nullptr,Texture* EmissionTexture = nullptr,Texture* NormalTexture = nullptr, Shader* shader = nullptr,
                      glm::vec3 localPos = glm::vec3(0.0f),
                      glm::vec3 localRot = glm::vec3(0.0f),
                      glm::vec3 localScale = glm::vec3(1.0f));

    // Add a child node
    void AddChildNode(const std::shared_ptr<SceneNode>& node);

    // Get child meshes
    std::vector<MeshNode>& GetChildMeshes();

    // Get child nodes
    std::vector<std::shared_ptr<SceneNode>>& GetChildNodes();

    // Remove mesh at index
    void RemoveChildMesh(size_t index);

    // Transform setters
    void SetPosition(glm::vec3 pos);
    void SetRotation(glm::vec3 rot);
    void SetScale(glm::vec3 scale);

    // Transform getters
    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::vec3 GetRotation() const;
    [[nodiscard]] glm::vec3 GetScale() const;

    // Translate/Rotate/Scale (additive)
    void Translate(glm::vec3 offset);
    void Rotate(glm::vec3 rotation);
    void Scale(glm::vec3 scale);
};

// Scene manager - contains root nodes
class Scenemap {
private:
    std::vector<std::shared_ptr<SceneNode>> rootNodes;

public:
    // Add a root node to the scene
    std::shared_ptr<SceneNode> AddRootNode(glm::vec3 pos = glm::vec3(0.0f),
                                           glm::vec3 rot = glm::vec3(0.0f),
                                           glm::vec3 scale = glm::vec3(1.0f));

    // Get all root nodes
    std::vector<std::shared_ptr<SceneNode>>& GetRootNodes();

    // Remove root node at index
    void RemoveRootNode(size_t index);

    // Clear all nodes
    void Clear();

    // Get node count
    [[nodiscard]] size_t GetRootNodeCount() const;
};


#endif //GRAPHICSENGINE_SCENEMAP_H