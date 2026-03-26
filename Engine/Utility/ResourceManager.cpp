
#include "ResourceManager.h"
#include "Texture.h"

std::unordered_map<std::string, Texture*> ResourceManager::textures_loaded;
std::unordered_map<std::string, Shader*> ResourceManager::shadersLoaded;

void ResourceManager::LoadTexture(const std::string &texture_path, const std::string &name) {
    Texture* tex = new Texture(texture_path);

    textures_loaded[name] = tex;
}

Texture *ResourceManager::GetTexture(const std::string &name) {
    auto it = textures_loaded.find(name);
    if (it != textures_loaded.end()) {
        return it->second;
    }

    return nullptr;

}

void ResourceManager::LoadShader(const std::string& shader_path, const std::string &name) {
    //Shader* tex = new Shader(shader_path, );

    //shadersLoaded[name] = tex;
}

Shader *ResourceManager::GetShader(const std::string &name) {
    auto it = shadersLoaded.find(name);
    if (it != shadersLoaded.end()) {
        return it->second;
    }

    return nullptr;

}