#version 330 core
#define MAX_LIGHTS 8

// ========== LIGHT TYPES ==========
// Must match LightType enum in Light.h
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

// ========== FRAGMENT OUTPUT ==========
out vec4 FragColor;  // Final color (RGBA) for this pixel

// ========== VERTEX SHADER INPUTS (interpolated across triangle) ==========
in vec3 ourColor;    // Interpolated vertex color
in vec2 TexCoord;    // Interpolated texture coordinate
in vec3 Normal;      // Interpolated normal in world space
in vec3 FragPos;     // Interpolated position in world space
in mat3 TBN;         // Tangent-Bitangent-Normal matrix (for normal mapping)

// ========== TEXTURE UNIFORMS ==========
uniform sampler2D ourTexture;   // Main diffuse texture
uniform sampler2D normalMap;    // Normal map for surface detail

// ========== MATERIAL/COLOR UNIFORMS ==========
uniform vec3 objectColor;       // Fallback color if no texture is used
uniform vec3 viewPos;           // Camera position in world space
uniform bool useTexture;        // Whether to use texture or solid color
uniform bool useNormalMap;      // Whether to use normal mapping

// ========== LIGHTING UNIFORMS ==========
uniform int numLights;          // Number of active lights (0 to MAX_LIGHTS)

// ========== LIGHT STRUCTURE ==========
// Matches Light struct in Light.h (packed for GPU memory)
struct Light {
    int type;                   // 0 = Directional, 1 = Point, 2 = Spot
    vec3 position;              // World position (unused for directional)
    vec3 ambient;               // Ambient color/intensity
    vec3 diffuse;               // Diffuse color/intensity
    vec3 specular;              // Specular color/intensity
    float constant;             // Attenuation constant (point/spot)
    float linear;               // Attenuation linear (point/spot)
    float quadratic;            // Attenuation quadratic (point/spot)
    vec3 direction;             // Direction for directional/spot lights
    float cutOff;               // Inner cutoff angle cosine (spot)
    float outerCutOff;          // Outer cutoff angle cosine (spot)
};

// ========== MATERIAL STRUCTURE ==========
struct Material {
    vec3 ambient;           // Ambient reflectance
    sampler2D diffuse;      // Diffuse color texture
    sampler2D specular;     // Specular/shininess texture
    float shininess;        // Shininess exponent
};

// ========== GLOBAL SHADER UNIFORMS ==========
uniform Light lights[MAX_LIGHTS];   // Array of up to 8 lights
uniform Material material;          // Material properties

// ========== HELPER FUNCTIONS ==========

// Calculate attenuation for point/spot lights using inverse quadratic formula
float CalcAttenuation(Light light, float distance) {
    return 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
}

// ========== DIRECTIONAL LIGHT CALCULATION ==========
// Directional lights come from infinity in a fixed direction
// Examples: Sun, Moon
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 albedo, float specMap) {
    // Light direction is from the light source direction (opposite of travel)
    vec3 lightDir = normalize(-light.direction);

    // ========== AMBIENT ==========
    vec3 ambient = light.ambient * albedo;

    // ========== DIFFUSE ==========
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo;

    // ========== SPECULAR ==========
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specMap;

    return ambient + diffuse + specular;
}

// ========== POINT LIGHT CALCULATION ==========
// Point lights emit light in all directions from a position
// Examples: Light bulb, Torch, Explosion
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float specMap) {
    // Direction from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);

    // Distance for attenuation calculation
    float distance = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light, distance);

    // ========== AMBIENT ==========
    vec3 ambient = light.ambient * albedo * attenuation;

    // ========== DIFFUSE ==========
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo * attenuation;

    // ========== SPECULAR ==========
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specMap * attenuation;

    return ambient + diffuse + specular;
}

// ========== SPOT LIGHT CALCULATION ==========
// Spot lights emit light in a cone from a position
// Examples: Flashlight, Spotlight on stage
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float specMap) {
    // Direction from fragment to light
    vec3 lightDir = normalize(light.position - fragPos);

    // Distance for attenuation
    float distance = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light, distance);

    // Check if fragment is within spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // ========== AMBIENT ==========
    vec3 ambient = light.ambient * albedo * attenuation * intensity;

    // ========== DIFFUSE ==========
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo * attenuation * intensity;

    // ========== SPECULAR ==========
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specMap * attenuation * intensity;

    return ambient + diffuse + specular;
}

// ========== MAIN FRAGMENT SHADER ==========
void main()
{
    // ========== NORMAL MAPPING ==========
    vec3 norm;
    if(useNormalMap) {
        norm = texture(normalMap, TexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);  // Convert [0,1] to [-1,1]
        norm = normalize(TBN * norm);        // Transform to world space
    } else {
        norm = normalize(Normal);
    }

    // ========== ALBEDO (BASE COLOR) ==========
    vec3 albedo = useTexture ? texture(material.diffuse, TexCoord).rgb : objectColor;

    // ========== SPECULAR MAP ==========
    float specMap = texture(material.specular, TexCoord).r;

    // ========== VIEW DIRECTION ==========
    vec3 viewDir = normalize(viewPos - FragPos);

    // ========== INITIALIZE OUTPUT COLOR ==========
    vec3 result = vec3(0.0);

    // ========== LOOP THROUGH ALL LIGHTS ==========
    for (int i = 0; i < numLights; i++) {
        // Dispatch to appropriate lighting function based on light type
        if (lights[i].type == LIGHT_DIRECTIONAL) {
            result += CalcDirLight(lights[i], norm, viewDir, albedo, specMap);
        }
        else if (lights[i].type == LIGHT_POINT) {
            result += CalcPointLight(lights[i], norm, FragPos, viewDir, albedo, specMap);
        }
        else if (lights[i].type == LIGHT_SPOT) {
            result += CalcSpotLight(lights[i], norm, FragPos, viewDir, albedo, specMap);
        }
    }

    // ========== OUTPUT FINAL COLOR ==========
    FragColor = vec4(result, 1.0);
}