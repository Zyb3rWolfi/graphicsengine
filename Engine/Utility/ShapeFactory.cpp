#include "ShapeFactory.h"
#include "../Renderer/Mesh.h"

ShapeFactory::ShapeFactory() {
}

Mesh ShapeFactory::CreateQuad() {
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    return Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
}

Mesh ShapeFactory::CreateCube() {
    float vertices[] = {
        // positions          // colors           // uv        // normals           // tangents
        // Front Face (Normal: 0, 0, -1) -> Tangent: (1, 0, 0)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,

        // Back Face (Normal: 0, 0, 1) -> Tangent: (-1, 0, 0)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,

        // Left Face (Normal: -1, 0, 0) -> Tangent: (0, 0, 1)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

        // Right Face (Normal: 1, 0, 0) -> Tangent: (0, 0, -1)
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

        // Top Face (Normal: 0, 1, 0) -> Tangent: (1, 0, 0)
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,

        // Bottom Face (Normal: 0, -1, 0) -> Tangent: (1, 0, 0)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        0, 2, 3,

        // Back face
        4, 5, 6,
        4, 6, 7,

        // Left face
        8, 9, 10,
        8, 10, 11,

        // Right face
        12, 13, 14,
        12, 14, 15,

        // Top face
        16, 17, 18,
        16, 18, 19,

        // Bottom face
        20, 21, 22,
        20, 22, 23

    };

    return Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
}
