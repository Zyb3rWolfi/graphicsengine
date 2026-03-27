# Code Commentary Summary

## Files Commented

All source files have been comprehensively commented explaining:
- What each class/function does
- What parameters do and how they're used
- Where functions link to other parts of the engine
- How data flows through the system

### Header Files (.h)

1. **Engine/Renderer/Mesh.h**
   - Explains material properties (ambient, diffuse, specular, shininess)
   - Documents GPU resources (VAO, VBO, EBO)
   - Explains why copy constructor is deleted
   - Documents move semantics for safe ownership transfer

2. **Engine/Scene/Scenemap.h**
   - Explains scene graph hierarchy and local vs world space
   - Documents MeshNode and SceneNode structures
   - Explains transform calculations
   - Documents all methods with parameters and usage

3. **Engine/Renderer/Renderer.h**
   - Explains rendering pipeline and scene graph traversal
   - Documents recursive rendering algorithm
   - Lists all uniform variables passed to shaders

### Implementation Files (.cpp)

1. **Engine/Core/Application.cpp**
   - Explains window creation and GLFW setup
   - Documents input mapping system
   - Explains main render loop structure
   - Documents scene setup and resource loading
   - Shows how meshes are moved to scene graph

2. **Engine/Utility/ShapeFactory.cpp**
   - Explains vertex data layout (56 bytes per vertex)
   - Documents cube geometry (24 vertices, 6 faces)
   - Shows index ordering and face winding
   - Explains normal and tangent directions per face

### Shader Files (.glsl)

1. **Shaders/shader.vert**
   - Explains vertex attribute mapping (locations 0-4)
   - Documents TBN matrix calculation
   - Explains Gram-Schmidt orthogonalization
   - Documents transform pipeline (model → view → projection)

2. **Shaders/shader.frag**
   - Explains normal mapping process
   - Documents Phong lighting model (ambient + diffuse + specular)
   - Shows how each light source contributes
   - Explains texture sampling and material properties

---

## Documentation Files Created

### ARCHITECTURE_GUIDE.md
Complete architecture documentation including:
- Core systems overview
- Data flow through the engine
- Scene graph hierarchy explanation
- Mesh geometry and GPU memory
- Rendering pipeline details
- Shader uniforms and variables
- Memory management patterns
- Troubleshooting guide
- Performance optimization tips

### QUICK_REFERENCE.md
Quick lookup guide including:
- File structure and locations
- Key classes and methods
- Common tasks (how to add meshes, change materials, load resources)
- Vertex data layout diagram
- Shader variable reference
- Lighting model equations
- Transform hierarchy
- Input handling
- Debug tips
- Common errors and fixes

---

## Key Concepts Explained

### 1. GPU Resource Management
- **Problem:** Original code passed mesh by pointer, causing crashes when local variable went out of scope
- **Solution:** Store meshes by value in MeshNode, use move semantics for safe ownership transfer
- **Explanation in code:** Mesh.h documents deleted copy constructor and move semantics

### 2. Scene Graph Hierarchy
- **Local Space:** Position/rotation relative to parent
- **World Space:** Absolute position in scene
- **Transformation:** WorldPos = ParentTransform × LocalTransform
- **Explanation in code:** Scenemap.h explains MeshNode and SceneNode hierarchy

### 3. Vertex Data Format
- **Layout:** 14 floats (56 bytes) per vertex
- **Structure:** Position(3) + Color(3) + TexCoord(2) + Normal(3) + Tangent(3)
- **GPU Layout:** Locations 0-4 mapped in Mesh.cpp SetupAttributes
- **Explanation in code:** ShapeFactory.cpp shows vertex array structure

### 4. Lighting Model (Phong)
- **Ambient:** Constant light (fills shadows)
- **Diffuse:** Scattered light based on surface angle (main brightness)
- **Specular:** Shiny highlights in light direction
- **Explanation in code:** shader.frag documents each component with equations

### 5. Normal Mapping
- **Purpose:** Add surface detail without extra geometry
- **TBN Matrix:** Converts tangent-space normals to world space
- **Implementation:** Built in vertex shader, used in fragment shader
- **Explanation in code:** shader.vert explains TBN calculation

---

## How to Use the Documentation

### I want to understand the overall architecture
→ Read ARCHITECTURE_GUIDE.md

### I want to add a new feature quickly
→ Use QUICK_REFERENCE.md "Common Tasks" section

### I want to understand how a specific file works
→ Read the comments in that file's header and implementation

### I want to understand the rendering pipeline
→ Read Renderer.h and shader files with comments

### I want to understand the scene graph
→ Read Scenemap.h comments and ARCHITECTURE_GUIDE.md

### I'm getting an error and need to debug
→ Check QUICK_REFERENCE.md "Common Errors" or ARCHITECTURE_GUIDE.md "Troubleshooting"

---

## Code Structure Overview

```
User Action (keyboard/mouse)
         ↓
Application::ProcessInput()
         ↓
Application::Update()
         ↓
Application::Render()
         ↓
Renderer::Draw(scene, camera, lights)
         ↓
Renderer::RenderNode() [RECURSIVE]
         ├─ Calculate world transform
         ├─ For each MeshNode:
         │  ├─ Bind shader
         │  ├─ Set uniforms
         │  └─ Mesh::Draw()
         │      ├─ Bind VAO/EBO
         │      └─ glDrawElements()
         │         ├─ Vertex Shader (1 per vertex)
         │         ├─ Rasterization
         │         └─ Fragment Shader (1 per pixel)
         │
         └─ For each child SceneNode:
            └─ Renderer::RenderNode() [RECURSION]
                     ↓
         GPU Framebuffer
                     ↓
         glfwSwapBuffers() → Display on screen
```

---

## Key Insights for Future Development

### 1. Move Semantics is Critical
The engine uses move semantics extensively to safely transfer GPU resource ownership. Always use `std::move()` when adding meshes to the scene.

### 2. Scene Graph is Hierarchical
Objects organize into parent-child relationships. Each node's transform is relative to its parent. This allows easy grouping and manipulation of objects.

### 3. Rendering is Recursive
The renderer recursively traverses the scene graph, calculating cumulative transforms. This is how complex hierarchies render correctly.

### 4. Uniforms Bridge C++ and GLSL
All communication between C++ and shaders happens through uniform variables. Adding new mesh properties requires:
- New uniform in shader
- setUniform() call in C++

### 5. Lighting Separates Concerns
Lights are independent from meshes. Each mesh can have different materials and shaders. Lights affect all meshes equally.

---

## Testing the Documentation

To verify the comments are clear, try this:
1. Pick a source file you've never seen
2. Read only the comments (skip the actual code)
3. Can you understand what the file does? ✓
4. Can you find where to add a new feature? ✓

If "yes" to both, the documentation is working!

---

## Next Steps for You

Now that the code is commented, you can:

1. **Add new mesh types** (Example: Sphere, Pyramid)
   - Create vertices in ShapeFactory
   - See ShapeFactory.cpp comments for format

2. **Add new materials** (Example: Glass, Metal)
   - Add properties to Mesh class
   - Update fragment shader to use them
   - See shader.frag comments for lighting model

3. **Add new light types** (Example: Directional, Spot)
   - Create Light subclass
   - Update shader loop
   - See shader.frag comments for current model

4. **Optimize rendering** (Example: Instancing, Culling)
   - Modify Renderer::RenderNode
   - See ARCHITECTURE_GUIDE.md for performance tips

5. **Add post-processing** (Example: Bloom, Shadows)
   - Create second rendering pass
   - Render to texture instead of screen
   - See Renderer.h for rendering pipeline

All the information you need is now in the comments and documentation!


