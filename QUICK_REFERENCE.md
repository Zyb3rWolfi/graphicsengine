# Quick Reference Guide

## File Structure

```
graphicsengine/
├── Engine/
│   ├── Core/
│   │   ├── Application.h/.cpp      ← Main engine class, render loop
│   │   └── Input.h/.cpp            ← Keyboard/mouse input handling
│   ├── Renderer/
│   │   ├── Renderer.h/.cpp         ← Scene graph traversal & rendering
│   │   ├── Mesh.h/.cpp             ← Geometry & GPU resources
│   │   ├── Shader.h/.cpp           ← GLSL shader compilation & uniforms
│   │   ├── Texture.h/.cpp          ← Texture loading
│   │   └── Vertex.h                ← Vertex data structure
│   ├── Scene/
│   │   ├── Scenemap.h/.cpp         ← Scene graph hierarchy
│   │   ├── Camera.h/.cpp           ← View/projection matrices
│   │   └── Light.h                 ← Light source definition
│   └── Utility/
│       ├── ShapeFactory.h/.cpp     ← Create cubes/quads
│       ├── ResourceManager.h/.cpp  ← Load shaders/textures
│       ├── EngineTime.h/.cpp       ← Delta time tracking
│       └── Input.h/.cpp            ← Input abstraction
├── Shaders/
│   ├── shader.vert                 ← Main vertex shader (transforms + TBN)
│   ├── shader.frag                 ← Main fragment shader (lighting)
│   ├── sourceShader.vert           ← Light visualization vertex shader
│   └── sourceShader.frag           ← Light visualization fragment shader
├── Images/                          ← Textures used by meshes
└── main.cpp                         ← Entry point
```

---

## Key Classes & Methods

### Application
```cpp
Application(width, height)           // Constructor
bool Init()                           // Initialize window & OpenGL
void Run()                            // Main render loop
void ProcessInput(dt)                 // Handle input
void Update(dt)                       // Update game state
void Render()                         // Render scene
```

### Renderer
```cpp
void Draw(scene, camera, lights)     // Render entire scene
void RenderNode(node, transform, camera, lights)  // Recursive node rendering
```

### Scenemap / SceneNode
```cpp
auto root = scene.AddRootNode(pos)   // Create root node
root->AddChildMesh(mesh, texture, ..., shader, position)  // Add mesh
root->AddChildNode(child)             // Add child node
root->Rotate(angles)                  // Rotate node
```

### Mesh
```cpp
Mesh(vertices[], indices[], vertexSize, indexSize)  // Constructor
void Draw()                           // Render mesh
void SetupAttributes()                // Configure GPU vertex format
```

### Shader
```cpp
Shader(vertPath, fragPath)            // Load & compile shaders
void use()                            // Make shader active
void setMat4(name, matrix)            // Set 4x4 matrix uniform
void setVec3(name, vector)            // Set 3D vector uniform
void setInt(name, value)              // Set integer uniform
```

### Camera
```cpp
glm::mat4 GetViewMatrix()             // Get view transform
glm::mat4 GetProjectionMatrix()       // Get projection transform
void ProcessKeyboard(direction, dt)   // Move camera
```

---

## Common Tasks

### Add a New Mesh to Scene
```cpp
// In Application::Run()
Mesh myMesh = ShapeFactory().CreateCube();
myMesh.objectColor = glm::vec3(1, 0, 0);  // Red

root->AddChildMesh(std::move(myMesh),
    ResourceManager::GetTexture("wall_map"),  // Texture
    nullptr,                                   // Specular
    nullptr,                                   // Emission
    ResourceManager::GetTexture("wall_normal"), // Normal
    ResourceManager::GetShader("MainShader"),   // Shader
    glm::vec3(5, 0, 0));                       // Position
```

### Change Mesh Material
```cpp
mesh.ambient = glm::vec3(0.1f, 0.1f, 0.1f);    // Dark shadows
mesh.diffuse = glm::vec3(0.8f, 0.9f, 0.8f);    // Bright scattered light
mesh.specular = glm::vec3(1.0f, 1.0f, 1.0f);   // Full white = very shiny
mesh.shininess = 64.0f;                         // High = sharp highlights
```

### Load Shader
```cpp
ResourceManager::LoadShader("Shaders/shader.vert", 
                           "Shaders/shader.frag", 
                           "MainShader");
Shader* shader = ResourceManager::GetShader("MainShader");
```

### Load Texture
```cpp
ResourceManager::LoadTexture("Images/wall/default.jpg", "wall_map");
Texture* tex = ResourceManager::GetTexture("wall_map");
```

### Add a Light
```cpp
// In Application constructor
lights = {
    Light(glm::vec3(-2.0f, 1.0f, -1.0f)),  // Light position
};
```

---

## Vertex Data Layout

Each vertex is 56 bytes:
```
Offset (bytes)  Size (bytes)  Data
0               12            Position (X, Y, Z)
12              12            Color (R, G, B)
24              8             TexCoord (U, V)
32              12            Normal (X, Y, Z)
44              12            Tangent (X, Y, Z)
```

In shader (location numbers):
```
Location 0: Position (3 floats, offset 0)
Location 1: Color (3 floats, offset 12)
Location 2: TexCoord (2 floats, offset 24)
Location 3: Normal (3 floats, offset 32)
Location 4: Tangent (3 floats, offset 44)
```

---

## Shader Variables

### Vertex Shader Inputs (from Mesh)
```glsl
layout (location = 0) in vec3 aPos;        // Vertex position
layout (location = 1) in vec3 aColor;      // Vertex color
layout (location = 2) in vec2 aTexCoord;   // Texture coordinate
layout (location = 3) in vec3 aNormal;     // Surface normal
layout (location = 4) in vec3 aTangent;    // Tangent for normal mapping
```

### Uniforms Set by C++ (Renderer.cpp)
```glsl
uniform mat4 model;              // Model to world transform
uniform mat4 view;               // World to camera transform
uniform mat4 projection;         // Camera to screen transform
uniform mat3 normalMatrix;       // Normal transformation

uniform int numLights;           // Number of active lights
uniform Light lights[MAX_LIGHTS]; // Light array
uniform vec3 viewPos;            // Camera position

uniform Material material;       // Material properties
uniform sampler2D normalMap;     // Normal map texture
uniform bool useTexture;         // Use texture or solid color
uniform bool useNormalMap;       // Use normal mapping
```

### Fragment Shader Output
```glsl
out vec4 FragColor;  // Final pixel color (RGBA)
```

---

## Lighting Model (Phong)

For each light:
```glsl
// Ambient: constant light from all directions
vec3 ambient = light.ambient * albedo;

// Diffuse: scattered light based on surface angle
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * diff * albedo;

// Specular: shiny highlights
float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
vec3 specular = light.specular * spec * specularMap;

// Combine
color += ambient + diffuse + specular;
```

---

## Transform Hierarchy

Local space (relative to parent):
```cpp
node->SetPosition(glm::vec3(5, 0, 0));     // 5 units right
node->SetRotation(glm::vec3(0, 45, 0));    // 45° around Y
node->SetScale(glm::vec3(2, 1, 1));        // 2x wide
```

Incremental transforms:
```cpp
node->Translate(glm::vec3(1, 0, 0));       // Move +1 unit right
node->Rotate(glm::vec3(0, 1, 0));          // Rotate +1° around Y
node->Scale(glm::vec3(1.1f, 1, 1));        // Scale 1.1x wider
```

---

## Input Handling

Mapped actions (in Application::Init):
```cpp
Input::MapAction("MoveForward", GLFW_KEY_W);
Input::MapAction("MoveForward", GLFW_KEY_UP);    // Multiple keys
Input::MapAction("MoveLeft", GLFW_KEY_A);
Input::MapAction("MoveRight", GLFW_KEY_D);
Input::MapAction("MoveBack", GLFW_KEY_S);
Input::MapAction("Quit", GLFW_KEY_ESCAPE);
```

Check in ProcessInput:
```cpp
if (Input::IsActionActive("MoveForward")) {
    camera.ProcessKeyboard(FORWARD, dt);
}
```

---

## Debug Output

Vertex struct size:
```cpp
std::cout << "Size of Vertex struct: " << sizeof(Vertex) << " bytes" << std::endl;
// Should print: 56 bytes
```

Shader compilation errors:
```cpp
std::cout << "Attempting to load shader from: " << path << std::endl;
std::cout << "Successfully loaded shader, size: " << ShaderCode.length() << " bytes" << std::endl;
```

---

## Memory Management

### Never do this (causes crash):
```cpp
Mesh cubeMain = factory.CreateCube();  // Local variable
// cubeMain goes out of scope here → GPU resources deleted
// But scene graph still has pointer to deleted memory ❌
```

### Always do this (safe):
```cpp
Mesh cubeMain = factory.CreateCube();  // Local variable
// Move ownership to scene graph
root->AddChildMesh(std::move(cubeMain), ...);
// Scene now owns the mesh ✓
```

---

## Common Errors

| Error | Cause | Fix |
|-------|-------|-----|
| Black screen | Shaders failed to compile | Check console for shader errors |
| White screen | Camera/lights not set up | Verify camera position and lights array |
| Crashing | Accessing invalid GPU resources | Verify meshes moved correctly to scene |
| Distorted model | Vertex layout mismatch | Check vertex stride = 56 bytes |
| No lighting | numLights = 0 | Verify lights array is populated |
| Texture not showing | Wrong texture unit | Check texture binding order in Renderer |

---

## Performance Checklist

- [ ] Only one Renderer::Draw() call per frame
- [ ] Shaders compiled once, reused for many meshes
- [ ] Textures cached in ResourceManager
- [ ] Reasonable number of lights (8 max)
- [ ] Scene hierarchy is not too deep
- [ ] Unused meshes removed from scene


