
#include "ResourceManager.h"
#include "Texture.h"

std::unordered_map<std::string, Texture*> ResourceManager::textures_loaded;
std::unordered_map<std::string, Shader*> ResourceManager::shadersLoaded;

void ResourceManager::LoadTexture(const std::string &texture_path, const std::string &name) {
    auto* tex = new Texture(texture_path);
    textures_loaded[name] = tex;
}

Texture *ResourceManager::GetTexture(const std::string &name) {
    auto it = textures_loaded.find(name);
    if (it != textures_loaded.end()) {
        return it->second;
    }

    return nullptr;

}

void ResourceManager::Clean() {

    // Getting the textures and shaders from the maps and deleting them to free memory
    for (auto& pair : textures_loaded) {
        delete pair.second;
    }
    textures_loaded.clear();

    for (auto& pair : shadersLoaded) {
        delete pair.second;
    }
    shadersLoaded.clear();
}

void ResourceManager::LoadShader(const std::string& vPath, const std::string& fPath, const std::string& name) {
    Shader* shader = new Shader(vPath.c_str(), fPath.c_str());
    shadersLoaded[name] = shader;
}

Shader* ResourceManager::GetShader(const std::string& name) {
    auto it = shadersLoaded.find(name);
    if (it != shadersLoaded.end()) {
        return it->second;
    }
    // Optional: Log an error if the shader wasn't found
    std::cout << "ERROR::RESOURCE_MANAGER::SHADER_NOT_FOUND: " << name << std::endl;
    return nullptr;
}