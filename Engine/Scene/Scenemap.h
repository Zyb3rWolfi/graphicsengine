//
// Created by Zyb3r on 13/02/2026.
//
// Scenemap.h - Defines the scene graph hierarchy for organizing 3D objects
// Scene graphs organize meshes into a tree structure with parent-child relationships
// Each node has local transforms that are relative to its parent (local space)
// The final world position is computed by combining all parent transforms (world space)
// LINKS TO: Application (builds scene), Renderer (traverses scene graph to render)

#ifndef GRAPHICSENGINE_SCENEMAP_H
#define GRAPHICSENGINE_SCENEMAP_H
#include "../Renderer/Mesh.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"

class GameObject;
// Forward declarations
class SceneNode;

// ========== MESHNODE STRUCT ==========
// MeshNode: Wraps a Mesh with its local transform and material properties
// This allows the same mesh to be rendered at different positions/rotations/scales
// Each mesh in the scene is stored as a MeshNode within a SceneNode
struct MeshNode {
    // ========== MATERIAL TEXTURES ==========
    // Each mesh can have up to 4 textures for different material properties:
    Texture* texture;           // Main diffuse texture (color map) - used for fragment color
    Texture* SpecularTexture;   // Specular map - defines which areas are shiny vs matte
    Texture* EmissionTexture;   // Emission map - areas that glow/emit light (unused currently)
    Texture* NormalTexture;     // Normal map - adds surface detail without extra geometry

    // ========== MESH & SHADER ==========
    Mesh mesh;           // The actual 3D mesh (stores geometry in GPU) - stored by value (owns it!)
    Shader* shader;      // Points to the shader program to use when rendering this mesh

    // ========== LOCAL TRANSFORM ==========
    // These are relative to the parent node - not world coordinates
    // Example: If parent is at (10,0,0) and this mesh's localPosition is (5,0,0),
    //          then world position = (15,0,0)
    glm::vec3 localPosition;    // Position offset from parent
    glm::vec3 localRotation;    // Rotation in degrees relative to parent
    glm::vec3 localScale;       // Scale multiplier relative to parent

    // ========== CONSTRUCTOR ==========
    // Parameters:
    //   m (Mesh&&) - mesh data, passed by rvalue reference (transfers ownership)
    //   t (Texture*) - main texture (default nullptr = no texture)
    //   st (Texture*) - specular texture (default nullptr = no specular)
    //   et (Texture*) - emission texture (default nullptr = no emission)
    //   nt (Texture*) - normal texture (default nullptr = no normal mapping)
    //   s (Shader*) - shader to use (default nullptr)
    //   pos (glm::vec3) - local position relative to parent (default 0,0,0)
    //   rot (glm::vec3) - local rotation in degrees (default 0,0,0)
    //   scale (glm::vec3) - local scale (default 1,1,1 = no scaling)
    // Flow: Called by SceneNode::AddChildMesh() -> creates MeshNode and adds to vector
    MeshNode(Mesh&& m, Texture* t = nullptr,Texture* st = nullptr,Texture* et = nullptr,Texture* nt = nullptr, Shader* s = nullptr, glm::vec3 pos = glm::vec3(0.0f),
             glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    // ========== METHODS ==========
    // GetLocalTransform: Builds a 4x4 matrix combining position, rotation, and scale
    // Returns: A transformation matrix that converts local coordinates to parent coordinates
    // Used by: Renderer::RenderNode() to calculate final world transform
    [[nodiscard]] glm::mat4 GetLocalTransform() const;
};

// ========== SCENENODE CLASS ==========
// SceneNode: Represents a node in the scene hierarchy
// Can contain meshes (MeshNode) and child nodes, forming a tree structure
// Example hierarchy:
//   Root (0,0,0)
//     ├─ House (5,0,0)
//     │   ├─ Mesh: Walls
//     │   └─ Door (1,0,0 relative to house)
//     └─ Car (10,0,0)
//         └─ Mesh: Vehicle
class SceneNode {
private:
    // ========== WORLD TRANSFORM (for this node's reference frame) ==========
    // These define where this node is relative to its parent
    glm::vec3 worldPosition;        // Position in parent's coordinate system
    glm::vec3 worldRotation;        // Rotation in degrees relative to parent
    glm::vec3 worldScale;           // Scaling relative to parent

    // ========== HIERARCHY DATA ==========
    std::vector<MeshNode> childMeshes;   // All meshes attached to this node (owns them by value)
    std::vector<std::shared_ptr<SceneNode>> childNodes;  // All child nodes (shared_ptr = shared ownership)

public:
    // ========== CONSTRUCTOR ==========
    // Creates a new scene node
    // Parameters:
    //   pos - position in parent space (default 0,0,0)
    //   rot - rotation in degrees (default 0,0,0)
    //   scale - scale factor (default 1,1,1)
    // Flow: Called by Application::Run() to create root node and child nodes
    SceneNode(glm::vec3 pos = glm::vec3(0.0f),
              glm::vec3 rot = glm::vec3(0.0f),
              glm::vec3 scale = glm::vec3(1.0f));

    std::shared_ptr<GameObject> gameObject;
    // ========== METHODS ==========
    // GetWorldTransform: Computes the 4x4 world transform matrix by combining
    //                    this node's and all parent nodes' transforms
    // Returns: A transformation matrix that converts local coordinates to world coordinates
    // Used by: Renderer::RenderNode() to get the final world transform for rendering
    [[nodiscard]] glm::mat4 GetWorldTransform() const;

    // AddChildMesh: Attach a new mesh to this node with its local transform
    // Parameters:
    //   mesh (Mesh&&) - mesh data, passed by rvalue reference (transfers ownership)
    //   texture (Texture*) - optional texture for the mesh (default nullptr)
    //   specularTexture (Texture*) - optional specular texture (default nullptr)
    //   EmissionTexture (Texture*) - optional emission texture (default nullptr)
    //   NormalTexture (Texture*) - optional normal map (default nullptr)
    //   shader (Shader*) - optional shader to use (default nullptr)
    //   localPos (glm::vec3) - local position relative to this node (default 0,0,0)
    //   localRot (glm::vec3) - local rotation in degrees (default 0,0,0)
    //   localScale (glm::vec3) - local scale relative to this node (default 1,1,1)
    // Flow: Called by Scenemap::AddRootNode() or SceneNode::AddChildNode()
    void AddChildMesh(Mesh&& mesh, Texture* texture = nullptr,Texture* specularTexture = nullptr,Texture* EmissionTexture = nullptr,Texture* NormalTexture = nullptr, Shader* shader = nullptr,
                      glm::vec3 localPos = glm::vec3(0.0f),
                      glm::vec3 localRot = glm::vec3(0.0f),
                      glm::vec3 localScale = glm::vec3(1.0f));

    // AddChildNode: Attach a new child node to this node
    // Parameters:
    //   node (std::shared_ptr<SceneNode>) - shared pointer to the child node
    // Flow: Called by SceneNode::AddChildMesh() or directly to build hierarchy
    void AddChildNode(const std::shared_ptr<SceneNode>& node);

    // GetChildMeshes: Access all child meshes
    // Returns: Reference to the vector of MeshNode objects
    // Used by: Renderer::RenderNode() to get meshes for rendering
    std::vector<MeshNode>& GetChildMeshes();

    // GetChildNodes: Access all child nodes
    // Returns: Reference to the vector of shared pointers to child SceneNode objects
    // Used by: Renderer::RenderNode() to traverse the scene graph
    std::vector<std::shared_ptr<SceneNode>>& GetChildNodes();

    // RemoveChildMesh: Detach and destroy the mesh at the given index
    // Parameters:
    //   index (size_t) - index of the mesh to remove
    // Flow: Called by application code to modify the scene
    void RemoveChildMesh(size_t index);

    // Transform setters: Update the transform of this node
    // Parameters:
    //   pos (glm::vec3) - new position (default glm::vec3(0.0f))
    //   rot (glm::vec3) - new rotation in degrees (default glm::vec3(0.0f))
    //   scale (glm::vec3) - new scale (default glm::vec3(1.0f))
    // Flow: Called by application code to move/rotate/scale nodes
    void SetPosition(glm::vec3 pos);
    void SetRotation(glm::vec3 rot);
    void SetScale(glm::vec3 scale);

    // Transform getters: Get the current transform of this node
    // Returns: The current position, rotation, or scale
    // Used by: Application code to query node transforms
    [[nodiscard]] glm::vec3& GetPosition();
    [[nodiscard]] glm::vec3& GetRotation();
    [[nodiscard]] glm::vec3& GetScale();

    // Translate/Rotate/Scale: Modify the transform by adding to the current values
    // Parameters:
    //   offset (glm::vec3) - translation offset
    //   rotation (glm::vec3) - rotation offset in degrees
    //   scale (glm::vec3) - scaling factor
    // Flow: Called by application code for incremental transformations
    void Translate(glm::vec3 offset);
    void Rotate(glm::vec3 rotation);
    void Scale(glm::vec3 scale);
};

// ========== SCENEMANAGER CLASS ==========
// Scenemap: Manages the entire scene graph
// Contains all root nodes of the scene hierarchy
// Responsible for adding/removing nodes and clearing the scene
// LINKS TO: Application (gets scene graph to build/render), Renderer (renders the scene)
class Scenemap {
private:
    std::vector<std::shared_ptr<SceneNode>> rootNodes;  // All root nodes in the scene

public:
    // ========== METHODS ==========
    // AddRootNode: Create and add a new root node to the scene
    // Parameters:
    //   pos (glm::vec3) - position of the root node (default 0,0,0)
    //   rot (glm::vec3) - rotation of the root node in degrees (default 0,0,0)
    //   scale (glm::vec3) - scale of the root node (default 1,1,1)
    // Returns: Shared pointer to the newly created root node
    // Flow: Called by Application::Run() to create the initial scene
    std::shared_ptr<SceneNode> AddRootNode(glm::vec3 pos = glm::vec3(0.0f),
                                           glm::vec3 rot = glm::vec3(0.0f),
                                           glm::vec3 scale = glm::vec3(1.0f));

    // GetRootNodes: Access all root nodes
    // Returns: Reference to the vector of shared pointers to root SceneNode objects
    // Used by: Application code to traverse the scene graph
    std::vector<std::shared_ptr<SceneNode>>& GetRootNodes();

    // RemoveRootNode: Detach and destroy the root node at the given index
    // Parameters:
    //   index (size_t) - index of the root node to remove
    // Flow: Called by application code to modify the scene
    void RemoveRootNode(size_t index);

    // Clear: Remove all nodes from the scene
    // Flow: Called by application code to reset the scene
    void Clear();

    // GetRootNodeCount: Get the number of root nodes
    // Returns: The number of root nodes in the scene
    // Used by: Application code to query the scene state
    [[nodiscard]] size_t GetRootNodeCount() const;
};


#endif //GRAPHICSENGINE_SCENEMAP_H

