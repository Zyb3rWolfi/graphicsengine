//
// Created by Zyb3r on 31/01/2026.
//

#ifndef GRAPHICSENGINE_TEXTURE_H
#define GRAPHICSENGINE_TEXTURE_H

#include <string>


class Texture {
public:
    unsigned int ID;
    unsigned char *data;
    unsigned int texture;
    int width, height, nrComponents;

    Texture(const std::string &fileName);

    unsigned int use() const;
};


#endif //GRAPHICSENGINE_TEXTURE_H