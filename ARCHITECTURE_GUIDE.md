# Graphics Engine Architecture Guide

## Overview
This is a modern C++20 graphics engine built with OpenGL 3.3, GLFW, and GLM. It uses a scene graph hierarchy for organizing 3D objects and implements advanced lighting with normal mapping.

---

## Core Systems

### 1. APPLICATION LAYER (Application.cpp/.h)
**Purpose:** Main engine class managing the entire system

**Responsibilities:**
- Window creation and GLFW/GLAD initialization
- Input handling and keyboard/mouse callbacks
- Scene setup and resource loading
- Main render loop

**Key Methods:**
- `Init()` - One-time initialization (window, OpenGL context, input mapping)
- `Run()` - Main loop: ProcessInput → Update → Render
- `ProcessInput(dt)` - Handle keyboard/mouse
- `Update(dt)` - Update game state (animations, transforms)
- `Render()` - Render the scene to screen

**Connections:**
```
Application
├─→ Renderer (draws scenes)
├─→ Scenemap (organizes objects)
├─→ Camera (view/projection)
├─→ ResourceManager (loads shaders/textures)
├─→ ShapeFactory (creates meshes)
└─→ Input (handles keyboard/mouse)
```

---

### 2. SCENE GRAPH (Scenemap.h/.cpp)
**Purpose:** Hierarchical organization of 3D objects

**Key Classes:**

#### MeshNode
Wraps a mesh with its local transform and material properties
```cpp
struct MeshNode {
    Mesh mesh;                      // The geometry
    Shader* shader;                 // Material/shader
    Texture* texture;               // Diffuse texture
    Texture* SpecularTexture;       // Specular map
    Texture* NormalTexture;         // Normal map
    Texture* EmissionTexture;       // Emission map
    
    // Transform relative to parent
    glm::vec3 localPosition;
    glm::vec3 localRotation;
    glm::vec3 localScale;
};
```

#### SceneNode
Node in the hierarchy containing meshes and child nodes
```cpp
class SceneNode {
    // Local transform (relative to parent)
    glm::vec3 worldPosition;
    glm::vec3 worldRotation;
    glm::vec3 worldScale;
    
    // Hierarchy
    std::vector<MeshNode> childMeshes;
    std::vector<std::shared_ptr<SceneNode>> childNodes;
};
```

**Hierarchy Example:**
```
Root (0, 0, 0)
├─ CubeMain (position: 2, 0, 0)
│  └─ Mesh: Wall texture + normal map
├─ CubeLight (position: -2, 1, -1)
│  └─ Mesh: Light source indicator
```

**Transform Calculation:**
```
Final World Position = ParentTransform * NodeLocalTransform * MeshLocalTransform
```

---

### 3. MESH & GEOMETRY (Mesh.h/.cpp)
**Purpose:** Stores vertex data and GPU resources

**Key Members:**
```cpp
class Mesh {
    unsigned int vao;  // Vertex Array Object (attribute config)
    unsigned int vbo;  // Vertex Buffer Object (vertex data)
    unsigned int ebo;  // Element Buffer Object (indices)
    
    int vertexCount;   // Total indices to render
    
    // Material properties
    glm::vec3 objectColor;
    glm::vec3 ambient, diffuse, specular;
    float shininess;
};
```

**Vertex Data Layout (56 bytes per vertex):**
```
Position(3)  |  Color(3)  |  TexCoord(2)  |  Normal(3)  |  Tangent(3)
12 bytes     |  12 bytes  |   8 bytes     |  12 bytes   |  12 bytes
Location 0   |  Location1 |  Location 2   |  Location 3 |  Location 4
```

**GPU Resource Management:**
- Destructor properly frees GPU memory
- Copy constructor DELETED (prevents dangling pointers)
- Move semantics enabled (safe transfer of ownership)

---

### 4. RENDERING (Renderer.h/.cpp)
**Purpose:** Traverses scene graph and renders meshes

**Key Algorithm:**
```cpp
Draw(scene, camera, lights)
├─ For each root node:
│  └─ RenderNode(node, identity matrix)
│     ├─ Calculate world transform
│     ├─ For each mesh in node:
│     │  ├─ Bind shader
│     │  ├─ Set uniforms (transforms, lights, textures)
│     │  └─ mesh.Draw()
│     └─ For each child node:
│        └─ RenderNode(child, world transform) [RECURSIVE]
```

**Uniform Variables Set Per Mesh:**
```glsl
Transforms:
- model (4x4)          // Local to world
- view (4x4)           // World to camera
- projection (4x4)     // Camera to screen
- normalMatrix (3x3)   // Normal transformation

Lighting:
- numLights (int)      // How many lights are active
- lights[8] (array)    // Light properties
- viewPos (vec3)       // Camera position

Material:
- material.diffuse (sampler2D)    // Color texture
- material.specular (sampler2D)   // Shininess map
- material.shininess (float)      // Highlight sharpness
- useTexture (bool)               // Use texture or solid color
- useNormalMap (bool)             // Use normal mapping
```

---

### 5. SHADERS

#### Vertex Shader (shader.vert)
**Inputs:** Vertex attributes from GPU buffer
- Position, Color, TexCoord, Normal, Tangent

**Outputs:** Interpolated values for fragment shader
- FragPos (world position)
- Normal (world normal)
- TexCoord (texture coordinates)
- TBN matrix (tangent space to world space)

**Key Operations:**
1. Transforms vertex position: model → world → view → screen
2. Calculates TBN matrix for normal mapping
3. Transforms normal to world space

#### Fragment Shader (shader.frag)
**Inputs:** Interpolated vertex shader outputs
- FragPos, Normal, TexCoord, TBN

**Outputs:** Final pixel color (RGBA)

**Key Operations:**
1. **Normal Mapping:** Converts tangent-space normal to world space using TBN
2. **Material:** Gets color from texture or solid color
3. **Lighting Loop:** For each light:
   - **Ambient:** Constant light in all directions
   - **Diffuse:** Light scattered based on surface angle
   - **Specular:** Shiny highlights based on reflection

---

### 6. RESOURCE MANAGER (ResourceManager.h/.cpp)
**Purpose:** Loads and caches shaders and textures

**Key Methods:**
```cpp
LoadShader(vertPath, fragPath, name);   // Load and cache shader
GetShader(name);                        // Retrieve cached shader

LoadTexture(path, name);                // Load and cache texture
GetTexture(name);                       // Retrieve cached texture

Clean();                                // Free all resources
```

---

### 7. SHAPE FACTORY (ShapeFactory.h/.cpp)
**Purpose:** Creates standard 3D shapes (Cube, Quad)

**Example: CreateCube()**
- Creates 24 vertices (6 faces × 4 vertices)
- Each vertex has full data: position, color, UVs, normals, tangents
- Returns Mesh by value (moved to caller)

---

## Data Flow: Rendering a Frame

```
1. APPLICATION.Run()
   │
   2. ProcessInput(dt)
   │  └─ Read keyboard/mouse → Update camera
   │
   3. Update(dt)
   │  └─ Rotate root nodes
   │
   4. Render()
   │  ├─ glClear() - Clear screen to dark blue
   │  ├─ Renderer.Draw(scene, camera, lights)
   │  │  └─ RenderNode() [RECURSIVE]
   │  │     ├─ Calculate world transform
   │  │     ├─ For each MeshNode:
   │  │     │  ├─ shader.use()
   │  │     │  ├─ glActiveTexture(0) + glBindTexture() - Bind textures
   │  │     │  ├─ shader.setMat4("model", transform)
   │  │     │  ├─ shader.setMat4("view", camera.GetViewMatrix())
   │  │     │  ├─ shader.setMat4("projection", camera.GetProjectionMatrix())
   │  │     │  ├─ Set light uniforms
   │  │     │  └─ mesh.Draw() - Vertex shader runs 24× (1 per vertex)
   │  │     │                    Fragment shader runs 1000s× (1 per pixel)
   │  │     └─ For each child node: RenderNode(child, world transform)
   │  │
   │  └─ GPU Pipeline:
   │     ├─ Vertex Shader
   │     ├─ Rasterization
   │     ├─ Fragment Shader
   │     └─ Framebuffer
   │
   5. glfwSwapBuffers() - Display rendered frame
   │
   6. glfwPollEvents() - Process OS events
   │
   [LOOP]
```

---

## Key Concepts

### Local vs World Space
- **Local Space:** Relative to parent node (local position/rotation)
- **World Space:** Absolute position in the scene
- **View Space:** Relative to the camera
- **Screen Space:** Final 2D coordinates on screen

### Transform Hierarchy
```
LocalMesh = Position_local + Rotation_local + Scale_local
WorldMesh = ParentTransform × LocalMesh
FinalMesh = ViewTransform × ProjectionTransform × WorldMesh
```

### Lighting Model (Phong)
```
FinalColor = AmbientLight + DiffuseLight + SpecularLight

Ambient = lightAmbient × albedo
Diffuse = lightDiffuse × max(dot(N, L), 0) × albedo
Specular = lightSpecular × pow(max(dot(V, R), shininess)) × specularMap
```

### Normal Mapping
- Stores surface detail normals in texture (RGB = XYZ)
- TBN matrix converts from tangent space → world space
- Allows detailed lighting without extra geometry

---

## Memory Management

### GPU Resources (Mesh class)
- **Owned:** VAO, VBO, EBO (freed in destructor)
- **Copy:** DELETED (prevents double-deletion)
- **Move:** Enabled (safe transfer of ownership)

### CPU Resources (Scene Graph)
- **Meshes:** Stored by value in MeshNode (owned)
- **Child nodes:** Shared pointers (reference counted)
- **Shaders/Textures:** Raw pointers (managed by ResourceManager)

---

## Adding New Features

### Add a New Mesh Type
1. Create vertices array in `ShapeFactory.cpp`
2. Create indices array defining triangles
3. Return `Mesh(vertices, indices, sizeof(vertices), sizeof(indices))`

### Add a New Material Property
1. Add member to Mesh class
2. Update vertex shader to calculate property
3. Update fragment shader to use property

### Add a New Light
1. Create Light object in Application
2. Add to `lights` vector
3. Fragment shader automatically uses it (up to MAX_LIGHTS)

### Add a New Scene Node
1. Create node: `auto child = std::make_shared<SceneNode>(position)`
2. Add mesh: `child->AddChildMesh(std::move(mesh), ...)`
3. Add to hierarchy: `parent->AddChildNode(child)`

---

## Troubleshooting

### Black screen
- Check that shaders compiled (check console for errors)
- Verify camera is positioned correctly
- Check that lights are active

### Crashing on render
- Verify texture files exist and paths are correct
- Check that shader uniform names match C++ code
- Ensure meshes have valid GPU resources (not moved twice)

### Distorted lighting
- Check normal map is loaded correctly
- Verify TBN matrix calculation in vertex shader
- Check that fragment shader is using correct light direction

---

## Performance Tips

1. **Frustum Culling:** Skip rendering nodes outside camera view
2. **Instancing:** Render same mesh multiple times efficiently
3. **LOD (Level of Detail):** Use simpler meshes for distant objects
4. **Deferred Rendering:** Separate lighting from geometry rendering
5. **Shader Optimization:** Minimize calculations in fragment shader


