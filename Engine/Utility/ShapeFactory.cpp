#include "ShapeFactory.h"
#include "../Renderer/Mesh.h"

// ========== SHAPEFACTORY CLASS ==========
// ShapeFactory: Utility class to generate standard 3D shapes as Mesh objects
// Creates vertices and indices for cubes, quads, and other shapes
// These meshes are used throughout the application for testing/rendering
// LINKS TO: Application (uses CreateCube()), Mesh (creates Mesh objects)

ShapeFactory::ShapeFactory() {
}

// ========== CREATE QUAD METHOD ==========
// CreateQuad: Creates a single quad (2 triangles, 4 vertices)
// Returns: A Mesh representing a flat square
// Geometry: Square in the XY plane at Z = -0.5 (faces toward +Z)
// Used for: Simple testing and potentially UI rendering
Mesh ShapeFactory::CreateQuad() {
    // Vertex array with 4 vertices
    // Each vertex has: Position(3) + Color(3) + TexCoord(2) + Normal(3) + Tangent(3) = 14 floats
    float vertices[] = {
        // Front quad (Z = -0.5)
        // Position              Color                 TexCoord    Normal               Tangent
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
    };

    // Index array: defines which vertices form triangles (groups of 3 indices = 1 triangle)
    // CCW winding order (counterclockwise when viewed from front)
    unsigned int indices[] = {
        0, 1, 3,  // First triangle (bottom-left, bottom-right, top-left)
        1, 2, 3   // Second triangle (bottom-right, top-right, top-left)
    };

    // Create and return mesh
    // sizeof(vertices) = 4 vertices * 14 floats * 4 bytes = 224 bytes
    // sizeof(indices) = 6 indices * 4 bytes = 24 bytes
    return Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
}

// ========== CREATE CUBE METHOD ==========
// CreateCube: Creates a complete 3D cube (6 faces, 24 vertices, 36 indices)
// Returns: A Mesh representing a unit cube centered at origin
// Geometry: Cube from (-0.5, -0.5, -0.5) to (0.5, 0.5, 0.5)
// Vertex Layout:
//   - Vertices 0-3: Front face (Z = -0.5)
//   - Vertices 4-7: Back face (Z = 0.5)
//   - Vertices 8-11: Left face (X = -0.5)
//   - Vertices 12-15: Right face (X = 0.5)
//   - Vertices 16-19: Top face (Y = 0.5)
//   - Vertices 20-23: Bottom face (Y = -0.5)
// Total: 24 vertices (each corner is duplicated for different face normals/UVs)
// Used for: Main rendering example, lighting tests, etc.
Mesh ShapeFactory::CreateCube() {
    float vertices[] = {
        // ========== FRONT FACE ==========
        // Normal: (0, 0, -1) pointing toward viewer, Tangent: (1, 0, 0) pointing right
        // Vertex position                Color           TexCoord    Normal               Tangent
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Vertex 0
         0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Vertex 1
         0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Vertex 2
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Vertex 3

        // ========== BACK FACE ==========
        // Normal: (0, 0, 1) pointing away from viewer, Tangent: (-1, 0, 0) pointing left
        -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Vertex 4
         0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Vertex 5
         0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Vertex 6
        -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Vertex 7

        // ========== LEFT FACE ==========
        // Normal: (-1, 0, 0) pointing left, Tangent: (0, 0, 1) pointing toward back
        -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Vertex 8
        -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Vertex 9
        -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Vertex 10
        -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Vertex 11

        // ========== RIGHT FACE ==========
        // Normal: (1, 0, 0) pointing right, Tangent: (0, 0, -1) pointing toward front
         0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 12
         0.5f, -0.5f,  0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 13
         0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 14
         0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // Vertex 15

        // ========== TOP FACE ==========
        // Normal: (0, 1, 0) pointing up, Tangent: (1, 0, 0) pointing right
        -0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Vertex 16
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Vertex 17
         0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Vertex 18
        -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Vertex 19

        // ========== BOTTOM FACE ==========
        // Normal: (0, -1, 0) pointing down, Tangent: (1, 0, 0) pointing right
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,     0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Vertex 20
         0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Vertex 21
         0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Vertex 22
        -0.5f, -0.5f,  0.5f,     0.0f, 1.0f, 1.0f,     0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f   // Vertex 23
    };

    // Index array: 6 faces * 2 triangles per face * 3 indices = 36 indices total
    // Winding order: CCW when viewed from outside the cube
    unsigned int indices[] = {
        // Front face (vertices 0-3)
        0, 1, 2,    // Triangle 1
        0, 2, 3,    // Triangle 2

        // Back face (vertices 4-7)
        4, 5, 6,
        4, 6, 7,

        // Left face (vertices 8-11)
        8, 9, 10,
        8, 10, 11,

        // Right face (vertices 12-15)
        12, 13, 14,
        12, 14, 15,

        // Top face (vertices 16-19)
        16, 17, 18,
        16, 18, 19,

        // Bottom face (vertices 20-23)
        20, 21, 22,
        20, 22, 23
    };

    // Create and return the cube mesh
    // sizeof(vertices) = 24 vertices * 14 floats * 4 bytes = 1344 bytes
    // sizeof(indices) = 36 indices * 4 bytes = 144 bytes
    return Mesh(vertices, indices, sizeof(vertices), sizeof(indices));
}
