#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform bool useTexture;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; uniform Light light;

uniform struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} material;

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;

    // Specular
    vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    // Get base color from texture or object color
    vec3 baseColor;

    if (useTexture) {
        baseColor = texture(ourTexture, TexCoord).rgb;
    } else {
        baseColor = objectColor;
    }

    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(ambient + diffuse + specular, 1.0);   }