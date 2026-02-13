//
// Created by Zyb3r on 06/02/2026.
//

#ifndef GRAPHICSENGINE_MESH_H
#define GRAPHICSENGINE_MESH_H


class Mesh {
    public:
    unsigned int vbo, vao, ebo;
    int vertexCount;
    Mesh(float vertices[], unsigned int indices[], unsigned int vertexSize, unsigned int indicesSize);

    Mesh(float * arr, unsigned int * indices);

    void Draw();
    void SetupAttributes();
};


#endif //GRAPHICSENGINE_MESH_H