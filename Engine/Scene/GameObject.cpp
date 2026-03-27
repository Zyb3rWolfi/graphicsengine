//
// Created by Zyb3r on 27/03/2026.
//

#include "GameObject.h"
#include "Scenemap.h"
#include "../Utility/ShapeFactory.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"
#include <memory>

// ========== CONSTRUCTOR ==========
GameObject::GameObject() : name(""), node(nullptr) {
    node = std::make_shared<SceneNode>();
}

// ========== FACTORY: CREATE CUBE ==========
// Creates a cube GameObject with proper mesh setup
shared_ptr<GameObject> GameObject::CreateCube(const string& name) {

    // GameObject is a wrapper around a SceneNode with a Mesh attached
    auto gameObject = make_shared<GameObject>();
    gameObject->name = name;

    // Create mesh geometry
    static ShapeFactory factory;
    Mesh cubeMesh = factory.CreateCube();

    // Add mesh to scene node using move semantics
    gameObject->node->AddChildMesh(std::move(cubeMesh),
        nullptr,  // texture
        nullptr,  // specularTexture
        nullptr,  // emissionTexture
        nullptr,  // normalTexture
        nullptr,  // shader
        glm::vec3(0.0f),  // position
        glm::vec3(0.0f),  // rotation
        glm::vec3(1.0f)   // scale
    );

    return gameObject;
}

// ========== FACTORY: CREATE QUAD ==========
shared_ptr<GameObject> GameObject::CreateQuad(const string& name) {
    auto gameObject = make_shared<GameObject>();
    gameObject->name = name;

    static ShapeFactory factory;
    Mesh quadMesh = factory.CreateQuad();

    gameObject->node->AddChildMesh(std::move(quadMesh),
        nullptr, nullptr, nullptr, nullptr, nullptr,
        glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)
    );

    return gameObject;
}

// ========== MATERIAL: SET TEXTURE ==========
void GameObject::SetTexture(Texture* texture) {
    if (!node->GetChildMeshes().empty()) {
        node->GetChildMeshes()[0].texture = texture;
    }
}

// ========== MATERIAL: SET SPECULAR TEXTURE ==========
void GameObject::SetSpecularTexture(Texture* texture) {
    if (!node->GetChildMeshes().empty()) {
        node->GetChildMeshes()[0].SpecularTexture = texture;
    }
}

// ========== MATERIAL: SET NORMAL TEXTURE ==========
void GameObject::SetNormalTexture(Texture* texture) {
    if (!node->GetChildMeshes().empty()) {
        node->GetChildMeshes()[0].NormalTexture = texture;
    }
}

// ========== MATERIAL: SET SHADER ==========
void GameObject::SetShader(Shader* shader) {
    if (!node->GetChildMeshes().empty()) {
        node->GetChildMeshes()[0].shader = shader;
    }
}

// ========== MATERIAL: SET COLOR ==========
void GameObject::SetColor(const glm::vec3& color) {
    if (!node->GetChildMeshes().empty()) {
        node->GetChildMeshes()[0].mesh.objectColor = color;
    }
}

// ========== TRANSFORM: SET POSITION ==========
void GameObject::SetPosition(const glm::vec3& pos) {
    node->SetPosition(pos);
}

// ========== TRANSFORM: SET ROTATION ==========
void GameObject::SetRotation(const glm::vec3& rot) {
    node->SetRotation(rot);
}

// ========== TRANSFORM: SET SCALE ==========
void GameObject::SetScale(const glm::vec3& scale) {
    node->SetScale(scale);
}

