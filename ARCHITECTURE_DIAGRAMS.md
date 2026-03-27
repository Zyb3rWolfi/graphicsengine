# Visual Architecture Diagrams

## 1. Rendering Pipeline

```
┌─────────────────────────────────────────────────────────────────────┐
│                      APPLICATION::RUN() LOOP                        │
└─────────────────────────────────────────────────────────────────────┘
                                  │
                    ┌─────────────┼─────────────┐
                    ▼             ▼             ▼
            ProcessInput()    Update()      Render()
                    │             │             │
                    └─────────────┼─────────────┘
                                  │
                    ┌─────────────▼──────────────┐
                    │   RENDERER::DRAW()         │
                    │  - Input: Scene, Camera,   │
                    │    Lights                  │
                    └─────────────┬──────────────┘
                                  │
              ┌───────────────────┴───────────────────┐
              ▼                                       ▼
     For each root node        (could be multiple)  RenderNode()
                                                     ↑      │
                                                     │      │ (recursive)
                    ┌────────────────────────────────┴──────┴──┐
                    │                                          │
            ┌───────▼────────┐                    ┌───────────▼──────┐
            │ Calculate      │                    │  For each child  │
            │ World          │                    │  node:           │
            │ Transform      │                    │  RenderNode()    │
            └────────┬───────┘                    │  (RECURSE)       │
                     │                            └──────────────────┘
        ┌────────────▼────────────┐
        │  For each MeshNode      │
        │  - Bind Shader          │
        │  - Set Uniforms         │
        │  - Bind Textures        │
        │  - Call Mesh::Draw()    │
        └────────────┬────────────┘
                     │
        ┌────────────▼──────────────┐
        │   GPU PIPELINE            │
        │                           │
        ├─ Vertex Shader ×24        │
        │  (1 per vertex)           │
        ├─ Rasterization           │
        ├─ Fragment Shader ×1000s   │
        │  (1 per pixel)            │
        └────────────┬──────────────┘
                     │
        ┌────────────▼──────────────┐
        │  Framebuffer/Backbuffer   │
        └────────────┬──────────────┘
                     │
        ┌────────────▼──────────────┐
        │  glfwSwapBuffers()        │
        │  (Display on screen)      │
        └───────────────────────────┘
```

---

## 2. Scene Graph Hierarchy

```
┌─────────────────────────────────────────────┐
│  ROOT NODE (position: 0, 0, 0)              │
│  [Contains 2 meshes and 0 child nodes]      │
│                                             │
│  ┌──────────────────────┐                   │
│  │ MeshNode 1: CubeMain │                   │
│  │ Position: (2, 0, 0)  │                   │
│  │ Rotation: (0, 0, 0)  │                   │
│  │ Scale: (1, 1, 1)     │                   │
│  │ Material: Wall       │                   │
│  │ Shader: MainShader   │                   │
│  └──────────────────────┘                   │
│                                             │
│  ┌──────────────────────┐                   │
│  │ MeshNode 2: CubeLight│                   │
│  │ Position: (-2, 1, -1)│                   │
│  │ Rotation: (0, 0, 0)  │                   │
│  │ Scale: (1, 1, 1)     │                   │
│  │ Material: White      │                   │
│  │ Shader: LightShader  │                   │
│  └──────────────────────┘                   │
└─────────────────────────────────────────────┘

Example hierarchical structure:
┌─ ROOT
│  ├─ CubeMain
│  │  ├─ [Mesh A]
│  │  └─ [Mesh B]
│  └─ CubeLight
│     └─ [Mesh C]
```

---

## 3. Data Flow: Adding a Mesh

```
┌─────────────────────┐
│ ShapeFactory        │
│ CreateCube()        │
│                     │
│ • Create 24 verts   │
│ • Create indices    │
│ • Return Mesh       │
└────────────┬────────┘
             │ (Mesh by value - no copy)
             ▼
      ┌──────────────────┐
      │ Local variable:  │
      │ cubeMain (Mesh)  │
      └────────┬─────────┘
               │ (std::move())
               ▼
    ┌──────────────────────┐
    │ SceneNode            │
    │ AddChildMesh()       │
    │ • MeshNode created   │
    │ • Mesh moved into    │
    │   MeshNode           │
    │ • Added to vector    │
    └──────────┬───────────┘
               │
               ▼
    ┌──────────────────────┐
    │ Scenemap             │
    │ childMeshes vector   │
    │ [MeshNode #1]        │
    │ [MeshNode #2]        │
    │ ...                  │
    └──────────────────────┘
               │
               ▼
    ┌──────────────────────┐
    │ Renderer             │
    │ RenderNode()         │
    │ Renders each mesh    │
    └──────────────────────┘
```

---

## 4. Memory Layout: Vertex (56 bytes)

```
┌─── VERTEX STRUCTURE (56 BYTES TOTAL) ───┐
│                                          │
│  0-11 bytes   [X][Y][Z]                  │
│  ┌────────────────────────┐              │
│  │  Position (vec3)       │ 3 floats     │
│  │  12 bytes              │              │
│  │  LOCATION 0            │              │
│  └────────────────────────┘              │
│                                          │
│  12-23 bytes  [R][G][B]                  │
│  ┌────────────────────────┐              │
│  │  Color (vec3)          │ 3 floats     │
│  │  12 bytes              │              │
│  │  LOCATION 1            │              │
│  └────────────────────────┘              │
│                                          │
│  24-31 bytes  [U][V]                     │
│  ┌────────────────────────┐              │
│  │  TexCoord (vec2)       │ 2 floats     │
│  │  8 bytes               │              │
│  │  LOCATION 2            │              │
│  └────────────────────────┘              │
│                                          │
│  32-43 bytes  [X][Y][Z]                  │
│  ┌────────────────────────┐              │
│  │  Normal (vec3)         │ 3 floats     │
│  │  12 bytes              │              │
│  │  LOCATION 3            │              │
│  └────────────────────────┘              │
│                                          │
│  44-55 bytes  [X][Y][Z]                  │
│  ┌────────────────────────┐              │
│  │  Tangent (vec3)        │ 3 floats     │
│  │  12 bytes              │              │
│  │  LOCATION 4            │              │
│  └────────────────────────┘              │
│                                          │
└──────────────────────────────────────────┘

Stride = 56 bytes
Total vertices in cube = 24
Total bytes for cube = 24 × 56 = 1344 bytes
```

---

## 5. Lighting Model (Phong)

```
┌─── PHONG LIGHTING MODEL ───┐
│                            │
│  FinalColor = Ambient +    │
│               Diffuse +    │
│               Specular     │
│                            │
└────────────┬───────────────┘
             │
    ┌────────┼────────┐
    ▼        ▼        ▼
┌──────┐ ┌────────┐ ┌─────────┐
│Ambient││Diffuse││Specular  │
└──────┘ └────────┘ └─────────┘
   │         │          │
   │         │          │
   ▼         ▼          ▼

Ambient:
light.ambient × albedo
(constant, fills shadows)

Diffuse:
light.diffuse × 
max(dot(N,L), 0) × 
albedo
(bright when facing light)

Specular:
light.specular × 
pow(max(dot(V,R), 0), shininess) × 
specularMap
(shiny highlights)

Where:
N = surface normal
L = light direction
V = view direction
R = reflected light
```

---

## 6. Transform Hierarchy

```
┌──────────────────────────────────┐
│  WORLD SPACE CALCULATION         │
└──────────────────────────────────┘

  Vertex Position (local space)
           │
           ▼
  ┌─────────────────────────┐
  │ Mesh Local Transform    │
  │ Position: (x, y, z)     │
  │ Rotation: (a, b, c)     │
  │ Scale: (sx, sy, sz)     │
  └────────────┬────────────┘
               │
               ▼
      Vertex in Parent Space
               │
               ▼
  ┌─────────────────────────┐
  │ Node Local Transform    │
  │ Position: (x, y, z)     │
  │ Rotation: (a, b, c)     │
  │ Scale: (sx, sy, sz)     │
  └────────────┬────────────┘
               │
               ▼
      Vertex in Grandparent Space
               │
               ▼
  ┌─────────────────────────┐
  │ (Continue recursively   │
  │  up the hierarchy)      │
  └────────────┬────────────┘
               │
               ▼
         WORLD SPACE
               │
               ▼
  ┌─────────────────────────┐
  │ View Transform          │
  │ (Camera look-at matrix) │
  └────────────┬────────────┘
               │
               ▼
       VIEW/CAMERA SPACE
               │
               ▼
  ┌─────────────────────────┐
  │ Projection Transform    │
  │ (Perspective matrix)    │
  └────────────┬────────────┘
               │
               ▼
     NORMALIZED DEVICE COORDS
     (Screen Space -1 to 1)
               │
               ▼
           SCREEN SPACE
```

---

## 7. Texture Units & Binding

```
┌─────────────────────────────────────────────────────┐
│         TEXTURE UNIT MAPPING                        │
├─────────────────────────────────────────────────────┤
│                                                     │
│ Texture Unit 0 (GL_TEXTURE0)                       │
│ ┌──────────────────────────────┐                   │
│ │ Diffuse Texture              │                   │
│ │ (Wall, color map)            │                   │
│ │ Shader: material.diffuse     │                   │
│ └──────────────────────────────┘                   │
│                                                     │
│ Texture Unit 1 (GL_TEXTURE1)                       │
│ ┌──────────────────────────────┐                   │
│ │ Specular Texture             │                   │
│ │ (Shininess map)              │                   │
│ │ Shader: material.specular    │                   │
│ └──────────────────────────────┘                   │
│                                                     │
│ Texture Unit 2 (GL_TEXTURE2)                       │
│ ┌──────────────────────────────┐                   │
│ │ Emission Texture             │                   │
│ │ (Glow map)                   │                   │
│ │ Shader: material.emission    │                   │
│ └──────────────────────────────┘                   │
│                                                     │
│ Texture Unit 3 (GL_TEXTURE3)                       │
│ ┌──────────────────────────────┐                   │
│ │ Normal Texture               │                   │
│ │ (Surface detail)             │                   │
│ │ Shader: normalMap            │                   │
│ └──────────────────────────────┘                   │
│                                                     │
└─────────────────────────────────────────────────────┘

C++ Code:
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, diffuseTexture);
shader.setInt("material.diffuse", 0);

glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularTexture);
shader.setInt("material.specular", 1);
```

---

## 8. TBN Matrix (Normal Mapping)

```
┌──────────────────────────────────────────┐
│      TANGENT SPACE → WORLD SPACE         │
│      Using TBN Matrix                    │
└──────────────────────────────────────────┘

Tangent Space:
┌─────────────────────┐
│  [0, 0, 1]          │
│   ↑                 │
│   │ (Z-up in texture)
│   │                 │
│ [←────────────]     │ (X-right in texture)
└─────────────────────┘

World Space:
        Y (up)
        │
        ├─→ X (right)
        │
        ▼
        Z (forward)

TBN Matrix Construction:
   T = tangent vector (X-axis in world)
   N = normal vector (Z-axis in world)  
   B = bitangent = cross(N, T) (Y-axis)

        ┌───────┐
TBN =   │T B N │
        └───────┘
        (3×3 matrix)

Transformation:
worldNormal = TBN × textureNormal
```

---

## 9. Shader Communication

```
┌────────────────────────────────────────────────────────┐
│  HOW C++ AND GLSL COMMUNICATE                          │
└────────────────────────────────────────────────────────┘

C++ Application                  GLSL Shaders
│                               │
├─ Create Shader ──────────→    ├─ Vertex Shader
│  (compile .vert/.frag)        │  (transforms vertices)
│                               │
├─ Bind Shader ────────────→    ├─ Active for rendering
│  (shader.use())               │
│                               │
├─ Set Uniforms ───────────→    ├─ Uniform Variables
│  (shader.setMat4(...))        │  (same for all vertices)
│  (shader.setVec3(...))        │
│                               │
├─ Bind VAO ───────────────→    ├─ Vertex Attributes
│  (vertex data)                │  (Location 0-4)
│                               │
├─ Draw Mesh ───────────────→   ├─ Fragment Shader
│  (glDrawElements())           │  (colors pixels)
│                               │
└────────────────────────────────────────────────────────┘

Uniform Variables (C++ → Shader):
- mat4 model, view, projection
- int numLights
- Light lights[8]
- Material material
- sampler2D diffuse, specular, normalMap
- bool useTexture, useNormalMap
```

---

## 10. CPU vs GPU Memory

```
┌──────────────────────────────────────────────────────────┐
│  MEMORY ALLOCATION                                       │
└──────────────────────────────────────────────────────────┘

CPU (System RAM)           GPU (VRAM)
├─ Application objects     ├─ Vertex Buffer (VBO)
│  - Scene graph           │  - 24 vertices × 56 bytes
│  - Node positions        │  - 1344 bytes total
│  - Transform matrices    │
│                          ├─ Index Buffer (EBO)
├─ Resource caches        │  - 36 indices
│  - Shaders              │  - 144 bytes total
│  - Textures             │
│                          ├─ Vertex Array (VAO)
├─ Frame data             │  - Vertex attribute config
│  - Camera position      │
│  - Light positions      ├─ Framebuffer
│  - Input state          │  - Color buffer (1024×768×4)
│                          │  - Depth buffer (1024×768×4)
└──────────────────────────────────────────────────────────┘

Data Transfer (CPU → GPU):
1. Create Mesh on CPU (vertices array)
2. Call glBufferData() - transfer to GPU VBO
3. GPU renders using VBO data
4. Result stored in framebuffer
5. glSwapBuffers() - display on screen
```

---

This file serves as a visual reference for understanding the architecture
without needing to read through code. Print these diagrams for quick reference!


