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

#include "Light.h"

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

    gameObject->node->gameObject = gameObject;  // Set back-reference for potential future use

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

shared_ptr<GameObject> GameObject::Create(const string &name, Scenemap *scene) {
    auto gameObject = make_shared<GameObject>();
    gameObject->name = name;
    gameObject->node->gameObject = gameObject;
    if (scene) {
        scene->GetRootNodes().push_back(gameObject->node);
    }
    return gameObject;
}

shared_ptr<GameObject> GameObject::CreatePointLight(const string &name, Scenemap *scene, const std::shared_ptr<SceneNode>& parent ) {
    auto lightObject = make_shared<GameObject>();

    lightObject->name = name;
    lightObject->node->gameObject = lightObject;
    lightObject->node->light = make_shared<Light>();
    lightObject->node->light->LightType = LightType::POINT;
    lightObject->node->light->position = glm::vec3(-2.0f, 1.0f, -1.0f);
    lightObject->node->light->ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    lightObject->node->light->diffuse = glm::vec3(0.8f, 0.7f, 0.5f);
    lightObject->node->light->specular = glm::vec3(1.0f, 1.0f, 0.8f);
    // Attenuation for ~50 unit range
    lightObject->node->light->constant = 1.0f;
    lightObject->node->light->linear = 0.09f;
    lightObject->node->light->quadratic = 0.032f;
    if (parent) {
        parent->AddChildNode(lightObject->node);
    } else {
        scene->GetRootNodes().push_back(lightObject->node);
    }
    lightObject->node->AddChildMesh(std::move(ShapeFactory().CreateCube()),
            nullptr,  // texture
            nullptr,  // specularTexture
            nullptr,  // emissionTexture
            nullptr,  // normalTexture
            nullptr,  // shader
            glm::vec3(0.0f),  // position
            glm::vec3(0.0f),  // rotation
            glm::vec3(1.0f)   // scale
        );
    return lightObject;
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

