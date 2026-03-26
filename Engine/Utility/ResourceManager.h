//
// Created by Zyb3r on 18/03/2026.
//

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"
using namespace std;


class ResourceManager {
public:
    static void LoadTexture(const std::string& texture_path, const std::string& name);
    static void LoadShader(const std::string&, const std::string &name);

    static Shader *GetShader(const std::string& name);
    static Texture *GetTexture(const std::string &name);
private:
    static std::unordered_map<std::string, Texture*> textures_loaded;
    static std::unordered_map<std::string, Shader*> shadersLoaded;
};



#endif //RESOURCEMANAGER_H
