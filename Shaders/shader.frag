#version 330 core
#define MAX_LIGHTS 8
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool useTexture;
uniform int numLights;
uniform bool useNormalMap;
uniform sampler2D normalMap;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Light lights[MAX_LIGHTS];
uniform Material material;


// shader.frag - Fragment Shader
// This shader is executed once per pixel (fragment)
// It calculates the final color by applying lighting, textures, and normal maps
// Uses Phong lighting model with multiple light sources
// LINKS FROM: Vertex shader (receives interpolated attributes)

// ========== MAXIMUM LIGHTS ==========
// Maximum number of lights that can affect the scene

// ========== FRAGMENT OUTPUT ==========
out vec4 FragColor;  // Final color (RGBA) for this pixel

// ========== VERTEX SHADER INPUTS (interpolated across triangle) ==========
in vec3 ourColor;    // Interpolated vertex color
in vec2 TexCoord;    // Interpolated texture coordinate
in vec3 Normal;      // Interpolated normal in world space
in vec3 FragPos;     // Interpolated position in world space
in mat3 TBN;         // Tangent-Bitangent-Normal matrix (for normal mapping)

// ========== TEXTURE UNIFORMS ==========
uniform sampler2D ourTexture;   // Main diffuse texture (unused, using material.diffuse instead)
uniform sampler2D normalMap;    // Normal map for surface detail

// ========== MATERIAL/COLOR UNIFORMS ==========
uniform vec3 objectColor;       // Fallback color if no texture is used
uniform vec3 viewPos;           // Camera position in world space (for specular calculations)
uniform bool useTexture;        // Whether to use texture or solid color
uniform bool useNormalMap;      // Whether to use normal mapping

// ========== LIGHTING UNIFORMS ==========
uniform int numLights;          // Number of active lights (0 to MAX_LIGHTS)

// ========== LIGHT STRUCTURE ==========
// Defines properties of each light source
struct Light {
    vec3 position;      // Position in world space
    vec3 ambient;       // Ambient color/intensity (light in all directions)
    vec3 diffuse;       // Diffuse color/intensity (direct scattered light)
    vec3 specular;      // Specular color/intensity (shiny highlights)
};

// ========== MATERIAL STRUCTURE ==========
// Defines surface material properties
struct Material {
    vec3 ambient;           // Ambient reflectance (how much ambient light is reflected)
    sampler2D diffuse;      // Diffuse color texture
    sampler2D specular;     // Specular/shininess texture (controls which areas are shiny)
    float shininess;        // Shininess exponent (higher = sharper, more reflective highlights)
};

// ========== GLOBAL SHADER UNIFORMS ==========
uniform Light lights[MAX_LIGHTS];  // Array of up to 8 lights
uniform Material material;         // Material properties for this mesh

// ========== MAIN FRAGMENT SHADER ==========
// Calculates lighting for each pixel using Phong lighting model
void main()
{
    // ========== NORMAL MAPPING ==========
    // Read the normal from the normal map or use the vertex normal
    vec3 norm;
    if(useNormalMap) {
        // Sample normal from texture (range 0-1, need to convert to -1 to 1)
        norm = texture(normalMap, TexCoord).rgb;
        // Convert from [0,1] to [-1,1]
        norm = normalize(norm * 2.0 - 1.0);
        // Transform from tangent space to world space using TBN matrix
        norm = normalize(TBN * norm);
    } else {
        // Use the interpolated vertex normal (already in world space)
        norm = normalize(Normal);
    }

    // ========== ALBEDO (BASE COLOR) ==========
    // Get the base color from either texture or solid color
    vec3 albedo = useTexture ? texture(material.diffuse, TexCoord).rgb : objectColor;

    // ========== LIGHTING COEFFICIENTS ==========
    // These could be uniforms but are hardcoded for now
    float ambientStrength = 0.1;      // How much ambient light affects the scene
    float specularStrength = 0.5;     // How strong specular highlights are (deprecated, use texture)

    // ========== INITIALIZE OUTPUT COLOR ==========
    // Start with black, accumulate contribution from each light
    vec3 result = vec3(0.0);

    // ========== LOOP THROUGH ALL LIGHTS ==========
    // Apply lighting from each active light source
    for (int i = 0; i < numLights; i++) {
        // Direction from fragment to light (normalized)
        vec3 lightDir = normalize(lights[i].position - FragPos);

        // Direction from fragment to camera/viewer (normalized)
        vec3 viewDir  = normalize(viewPos - FragPos);

        // Direction of reflected light (for specular calculations)
        vec3 reflectDir = reflect(-lightDir, norm);

        // ========== AMBIENT LIGHTING ==========
        // Ambient light is constant in all directions
        // Simulates indirect/bounced light that fills shadows
        vec3 ambient = lights[i].ambient * albedo;

        // ========== DIFFUSE LIGHTING ==========
        // How much light is scattered in all directions
        // Depends on angle between surface normal and light direction
        float diff = max(dot(norm, lightDir), 0.0);  // Clamp to 0 (never negative)
        vec3 diffuse = lights[i].diffuse * diff * albedo;

        // ========== SPECULAR LIGHTING ==========
        // Shiny highlights in the direction of reflected light
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // Get specular strength from specular map texture
        vec3 specMap = texture(material.specular, TexCoord).rgb;
        vec3 specular = lights[i].specular * spec * specMap;

        // ========== ACCUMULATE LIGHT CONTRIBUTION ==========
        // Add this light's contribution to the final result
        result += ambient + diffuse + specular;
    }

    // ========== OUTPUT FINAL COLOR ==========
    // Set fragment color with full opacity (alpha = 1.0)
    FragColor = vec4(result, 1.0);
}