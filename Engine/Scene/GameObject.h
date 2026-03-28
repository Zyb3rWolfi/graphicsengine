//
// Created by Zyb3r on 27/03/2026.
//
#ifndef GRAPHICSENGINE_GAMEOBJECT_H
#define GRAPHICSENGINE_GAMEOBJECT_H

#include <memory>
#include <string>
#include <glm/glm.hpp>

class Scenemap;
// Forward declarations
class SceneNode;
class Texture;
class Shader;
class Mesh;

using namespace std;

// ========== GAMEOBJECT CLASS ==========
// Wraps a SceneNode and Mesh for easy management
// Similar to Unity GameObject pattern
class GameObject : public std::enable_shared_from_this<GameObject> {
public:
    // Constructor
    GameObject();
    ~GameObject() = default;

    // Properties
    string name;
    std::shared_ptr<SceneNode> node;

    // Factory method
    static shared_ptr<GameObject> Create(const string& name, Scenemap *scene = nullptr);
    static shared_ptr<GameObject> CreateCube(const string& name);
    static shared_ptr<GameObject> CreateQuad(const string& name);
    static shared_ptr<GameObject> CreatePointLight(const string& name, Scenemap *scene = nullptr, const std::shared_ptr<SceneNode>& parent = nullptr);

    // Material/Texture setters
    void SetTexture(Texture* texture);
    void SetSpecularTexture(Texture* texture);
    void SetNormalTexture(Texture* texture);
    void SetShader(Shader* shader);
    void SetColor(const glm::vec3& color);

    // Transform helpers
    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);
    void SetScale(const glm::vec3& scale);
};


#endif //GRAPHICSENGINE_GAMEOBJECT_H