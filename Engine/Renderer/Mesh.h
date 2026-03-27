//
// Created by Zyb3r on 06/02/2026.
//
// Mesh.h - Represents a 3D mesh object with vertex data and OpenGL resources
// A mesh contains vertex positions, colors, texture coordinates, normals, and tangents
// It manages OpenGL buffers (VAO, VBO, EBO) for rendering
// LINKS TO: ShapeFactory (creates meshes), Renderer (draws meshes), Scenemap (stores meshes)

#ifndef GRAPHICSENGINE_MESH_H
#define GRAPHICSENGINE_MESH_H
#include <glm/fwd.hpp>

#include "Shader.h"


class Mesh {
public:
    // ========== MATERIAL PROPERTIES ==========
    // These control how light interacts with the mesh
    glm::vec3 objectColor;   // Base RGB color of the mesh
    glm::vec3 lightColor;    // Color of light (mainly for testing)
    glm::vec3 ambient;       // Ambient component: how much light reflects when there's no direct light
    glm::vec3 diffuse;       // Diffuse component: scattered light from all directions
    glm::vec3 specular;      // Specular component: shiny reflections in light direction
    float shininess;         // Shininess exponent: higher = sharper, shinier highlights

    // ========== OPENGL GPU RESOURCES ==========
    unsigned int vbo;  // Vertex Buffer Object - stores vertex data (position, color, texCoord, normal, tangent)
    unsigned int vao;  // Vertex Array Object - stores vertex attribute configuration/format
    unsigned int ebo;  // Element Buffer Object - stores indices (which vertices form triangles)

    // ========== MESH DATA ==========
    int vertexCount;   // Total number of indices (number of vertices to render)

    // ========== CONSTRUCTORS ==========
    // Main constructor: Creates a mesh from raw vertex and index data
    // Parameters:
    //   vertices[] - array of floats with layout per vertex: Position(3) + Color(3) + TexCoord(2) + Normal(3) + Tangent(3) = 14 floats
    //   indices[] - array of unsigned ints defining triangle connectivity (groups of 3 = one triangle)
    //   vertexSize - total byte size of vertices array (sizeof(vertices))
    //   indicesSize - total byte size of indices array (sizeof(indices))
    // Flow: Called by ShapeFactory::CreateCube() -> stores data in GPU -> SetupAttributes() configures layout
    Mesh(float vertices[], unsigned int indices[], unsigned int vertexSize, unsigned int indicesSize);

    // Alternative constructor (currently unused in codebase)
    Mesh(float *arr, unsigned int *indices);

    // ========== DESTRUCTOR & MOVE SEMANTICS ==========
    // Destructor: Cleans up GPU resources when this Mesh object is destroyed
    // Calls glDeleteVertexArrays/glDeleteBuffers to free VRAM
    // IMPORTANT: Each Mesh owns ONE set of GPU resources - when destroyed, they're freed
    ~Mesh();

    // Delete copy constructor - Prevents shallow copying of GPU handles
    // If we allowed copying, both Mesh objects would point to same GPU memory
    // When one dies, it deletes shared GPU resources, leaving the copy with dangling handles
    // This was causing the original crash!
    Mesh(const Mesh&) = delete;

    // Delete copy assignment operator - Same reason as copy constructor
    Mesh& operator=(const Mesh&) = delete;

    // Move constructor: Transfers GPU resource ownership from temporary Mesh
    // Used when returning Mesh from function: Mesh m = factory.CreateCube();
    // Takes resources from 'other' and invalidates 'other' so it doesn't double-delete on destruction
    Mesh(Mesh&& other) noexcept;

    // Move assignment: Same as move constructor but for assignment
    // Used when: m1 = std::move(m2);  // m1 takes m2's resources, m1's old resources are freed
    Mesh& operator=(Mesh&& other) noexcept;

    // ========== METHODS ==========
    // Draw: Renders this mesh to the currently bound framebuffer
    // Binds VAO and EBO, then issues glDrawElements to GPU
    // Called by: Renderer::RenderNode() for each mesh in the scene graph
    // Preconditions: A shader must be bound before calling
    void Draw();

    // SetupAttributes: Configures GPU on how to interpret vertex data in VBO
    // Sets up vertex attribute pointers for each data type:
    //   Location 0: Position (3 floats)
    //   Location 1: Color (3 floats)
    //   Location 2: TexCoord (2 floats)
    //   Location 3: Normal (3 floats)
    //   Location 4: Tangent (3 floats)
    // Called automatically in constructor during mesh creation
    // Stride = 56 bytes (14 floats * 4 bytes each)
    void SetupAttributes();

    // SetLighting: Applies lighting color uniforms to a shader (deprecated/not currently used)
    // Parameters:
    //   lightColor - the color of the light source
    //   objectColor - the base color of this mesh
    //   shader - the shader program to set uniforms on
    void SetLighting(glm::vec3 lightColor, glm::vec3 objectColor, Shader shader);
};


#endif //GRAPHICSENGINE_MESH_H