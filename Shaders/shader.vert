#version 330 core

// ========== VERTEX INPUT ATTRIBUTES ==========
// These correspond to the vertex layout from Mesh:
// Location 0: Position
layout (location = 0) in vec3 aPos;        // Vertex position in model space (-0.5 to 0.5)
layout (location = 1) in vec3 aColor;      // Vertex color (RGB 0-1)
layout (location = 2) in vec2 aTexCoord;   // Texture coordinate (UV 0-1)
layout (location = 3) in vec3 aNormal;     // Surface normal direction (normalized)
layout (location = 4) in vec3 aTangent;    // Tangent direction for normal mapping (normalized)

// ========== VERTEX OUTPUT (passed to Fragment Shader) ==========
out mat3 TBN;       // Tangent-Bitangent-Normal matrix (converts tangent space to world space)
out vec3 ourColor;  // Vertex color (interpolated across face)
out vec3 FragPos;   // Vertex position in world space (for lighting calculations)
out vec2 TexCoord;  // Texture coordinate (interpolated across face)
out vec3 Normal;    // Surface normal in world space

// ========== UNIFORMS (same for all vertices in a draw call) ==========
uniform mat4 model;         // Transforms from model space to world space
uniform mat4 view;          // Transforms from world space to camera/view space
uniform mat4 projection;    // Transforms from view space to clip space (screen space)
uniform mat3 normalMatrix;  // Special matrix for transforming normals (inverse transpose of model)

// ========== MAIN SHADER FUNCTION ==========
void main()
{
    // ========== TBN MATRIX CALCULATION ==========
    // TBN converts vectors from tangent space (used in normal maps) to world space
    // This is needed to properly apply normal maps

    // Transform tangent to world space and normalize
    vec3 T = normalize(mat3(model) * aTangent);
    // Transform normal to world space and normalize
    vec3 N = normalize(mat3(model) * aNormal);

    // Gram-Schmidt orthogonalization: ensure T and N are perpendicular
    // Sometimes interpolation can make them not perfectly perpendicular
    T = normalize(T - dot(T, N) * N);

    // Calculate bitangent from cross product of normal and tangent
    vec3 B = cross(N, T);

    // Build the TBN matrix (columns are T, B, N)
    // This matrix will convert tangent-space normals to world space
    TBN = mat3(T, B, N);

    // ========== POSITION TRANSFORMATION ==========
    // Transform vertex through the full pipeline:
    // 1. model: local space -> world space
    // 2. view: world space -> camera/view space
    // 3. projection: camera space -> clip space (NDC -1 to 1)
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // ========== OUTPUT CALCULATIONS ==========
    // These are interpolated across the triangle for the fragment shader

    // Calculate world space position (used for lighting calculations)
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal to world space using the special normal matrix
    // We use normalMatrix instead of just mat3(model) because uniform scaling could break normals
    Normal = normalMatrix * aNormal;

    // Pass through color and texture coordinates unchanged
    ourColor = aColor;
    TexCoord = aTexCoord;
}