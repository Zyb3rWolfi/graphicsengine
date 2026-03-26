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
}; uniform Light lights[MAX_LIGHTS];

uniform struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} material;


void main()
{
    vec3 norm;
    if(useNormalMap) {
        // Sample and transform from [0,1] to [-1,1]
        norm = texture(normalMap, TexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        // Use TBN to move the normal from tangent space to world space
        norm = normalize(TBN * norm);
    } else {
        norm = normalize(Normal); // Fallback to vertex normal
    }

    vec3 albedo = useTexture ? texture(material.diffuse, TexCoord).rgb : objectColor;

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    // Diffuse
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 viewDir  = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        // Ambient
        vec3 ambient = lights[i].ambient * albedo;
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuse * diff * albedo;

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specMap = texture(material.specular, TexCoord).rgb;
        vec3 specular = lights[i].specular * spec * specMap;

        // Get base color from texture or object color
        result += ambient + diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}