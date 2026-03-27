# Graphics Engine Documentation Index

Welcome! This directory now contains comprehensive documentation and comments explaining every part of your graphics engine.

## 📚 Documentation Files

### Start Here
- **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Fast lookup for common tasks and code locations
  - File structure
  - Key classes and methods
  - Common tasks with examples
  - Shader variable reference
  - Common errors and fixes

### Deep Dives
- **[ARCHITECTURE_GUIDE.md](ARCHITECTURE_GUIDE.md)** - Complete system architecture
  - Core systems overview
  - Data flow through the engine
  - Scene graph explanation
  - Rendering pipeline
  - Memory management
  - Troubleshooting guide

### Visual Learning
- **[ARCHITECTURE_DIAGRAMS.md](ARCHITECTURE_DIAGRAMS.md)** - ASCII diagrams of key systems
  - Rendering pipeline flow
  - Scene graph hierarchy
  - Data flow diagrams
  - Memory layouts
  - Lighting model
  - Transform calculations

### Summary
- **[COMMENTARY_SUMMARY.md](COMMENTARY_SUMMARY.md)** - Overview of all comments added
  - Files that were commented
  - Key concepts explained
  - How to use the documentation
  - Next steps for development

---

## 💻 Source Code Comments

All source code files now have detailed comments:

### Header Files (.h)
- `Engine/Renderer/Mesh.h` - GPU geometry and resources
- `Engine/Renderer/Renderer.h` - Rendering system
- `Engine/Scene/Scenemap.h` - Scene graph hierarchy

### Implementation Files (.cpp)
- `Engine/Core/Application.cpp` - Main engine and render loop
- `Engine/Utility/ShapeFactory.cpp` - Mesh creation with vertex layouts
- (Other files have minimal but clear comments)

### Shader Files (.glsl)
- `Shaders/shader.vert` - Vertex transformation and TBN calculation
- `Shaders/shader.frag` - Phong lighting model implementation

---

## 🎯 Quick Navigation

### By Task
| What I Want to Do | Where to Look |
|-------------------|---------------|
| Add a new mesh | QUICK_REFERENCE.md → "Add a New Mesh" |
| Change material properties | QUICK_REFERENCE.md → "Change Mesh Material" |
| Understand the render loop | ARCHITECTURE_GUIDE.md → "Data Flow" |
| Add a new light | QUICK_REFERENCE.md → "Add a Light" |
| Load textures/shaders | QUICK_REFERENCE.md → "Load Shader/Texture" |
| Fix rendering issues | QUICK_REFERENCE.md → "Common Errors" |
| Understand scene graph | ARCHITECTURE_DIAGRAMS.md → "Scene Graph Hierarchy" |
| Learn about memory management | ARCHITECTURE_GUIDE.md → "Memory Management" |

### By System
| System | Files | Documentation |
|--------|-------|----------------|
| Rendering | Renderer.h/.cpp, Mesh.h/.cpp | ARCHITECTURE_GUIDE.md → "Rendering" |
| Scene Graph | Scenemap.h/.cpp | ARCHITECTURE_GUIDE.md → "Scene Graph" |
| GPU Resources | Mesh.h/.cpp | Mesh.h comments |
| Shaders | shader.vert/.frag | Shader file comments |
| Application | Application.h/.cpp | Application.cpp comments |
| Resources | ResourceManager.h/.cpp | QUICK_REFERENCE.md → "Load Shader/Texture" |

---

## 📖 Reading Guide

### For Beginners
1. Read **QUICK_REFERENCE.md** to understand the file layout
2. Look at **ARCHITECTURE_DIAGRAMS.md** to visualize the systems
3. Read **Mesh.h** and **Application.cpp** comments to understand basics

### For Understanding Rendering
1. Read **ARCHITECTURE_GUIDE.md** → "Rendering Pipeline"
2. Look at **ARCHITECTURE_DIAGRAMS.md** → "Rendering Pipeline" diagram
3. Read **Renderer.h** comments to understand algorithm
4. Read **shader.vert** and **shader.frag** comments

### For Understanding Scene Graph
1. Read **ARCHITECTURE_DIAGRAMS.md** → "Scene Graph Hierarchy"
2. Read **Scenemap.h** comments (very detailed)
3. See **ARCHITECTURE_GUIDE.md** → "Scene Graph" section
4. Read example in **Application.cpp** run() function

### For Understanding Lighting
1. Read **shader.frag** comments → Phong lighting section
2. Look at **ARCHITECTURE_DIAGRAMS.md** → "Lighting Model" diagram
3. See **shader.frag** code with comments showing each component

### For Adding New Features
1. Find relevant section in **QUICK_REFERENCE.md** → "Common Tasks"
2. Follow the example code provided
3. Refer to **ARCHITECTURE_GUIDE.md** if you need system overview

---

## 🔍 Key Concepts Explained

### 1. **Scene Graph** (Hierarchical Object Organization)
```
Root
├─ CubeMain (position: 2, 0, 0)
│  └─ Mesh with Wall texture
└─ CubeLight (position: -2, 1, -1)
   └─ Mesh with Light shader
```
→ See: ARCHITECTURE_DIAGRAMS.md → "Scene Graph Hierarchy"

### 2. **Rendering Pipeline** (How a frame is rendered)
```
ProcessInput → Update → Render:
  Renderer::Draw(scene) 
  → RenderNode(recursive)
    → For each mesh: set uniforms → Mesh::Draw()
      → GPU: Vertex Shader → Rasterization → Fragment Shader
```
→ See: ARCHITECTURE_DIAGRAMS.md → "Rendering Pipeline"

### 3. **Vertex Data Layout** (How vertex info is organized)
```
[Position 3×float] [Color 3×float] [TexCoord 2×float] [Normal 3×float] [Tangent 3×float]
= 56 bytes total per vertex
```
→ See: QUICK_REFERENCE.md → "Vertex Data Layout"

### 4. **Move Semantics** (Safe GPU resource transfer)
```cpp
Mesh cube = ShapeFactory::CreateCube();
root->AddChildMesh(std::move(cube), ...);  // Transfers ownership
// cube is now invalid, but scene owns the GPU resources ✓
```
→ See: Mesh.h comments about move semantics

### 5. **Normal Mapping** (Surface detail without extra vertices)
```
Normal Map (texture) 
→ TBN matrix (in vertex shader)
→ Transform to world space
→ Use in lighting calculations (fragment shader)
```
→ See: shader.vert and shader.frag comments

---

## 🚀 Next Steps

### Learn & Understand (Recommended Order)
1. ✓ Read QUICK_REFERENCE.md (5 min)
2. ✓ View ARCHITECTURE_DIAGRAMS.md (10 min)
3. ✓ Read Mesh.h, Scenemap.h comments (20 min)
4. ✓ Read Application.cpp comments (20 min)
5. ✓ Read shader comments (15 min)

### Make Changes
1. Try adding a new mesh type (Sphere, Pyramid)
   - See ShapeFactory.cpp for vertex format
2. Try changing material properties
   - See Mesh class properties
3. Try adding a new light
   - See Application constructor
4. Try creating a hierarchy
   - See Application::Run() scene setup

### Advanced
1. Add post-processing effects
2. Implement frustum culling
3. Add LOD (level of detail)
4. Implement deferred rendering
5. Add shadow mapping

---

## 📝 Comment Format Guide

All comments follow this format:

```cpp
// ========== SECTION HEADER ==========
// Brief description of what this section does
// 
// Important details:
// - Point 1
// - Point 2
// 
// How it links to other parts:
// - Uses class X for Y
// - Called by function Z

function_or_class_name() {
    // Line-by-line explanation of logic
}
```

---

## ❓ FAQ

**Q: I'm confused about how meshes are stored. Where do I look?**
A: Read Mesh.h comments and ARCHITECTURE_DIAGRAMS.md → "Memory Layout"

**Q: How do I add a light that affects only certain meshes?**
A: Currently all lights affect all meshes. See QUICK_REFERENCE.md for how to modify the lighting loop.

**Q: Why do I need to use std::move() when adding meshes?**
A: Read Mesh.h comments about move semantics and memory management safety.

**Q: How do normals work?**
A: Read shader.frag comments about normal mapping and the TBN matrix.

**Q: Where do I find where shaders get uniforms set?**
A: Renderer.cpp RenderNode() function - it calls shader.setMat4(), shader.setVec3(), etc.

**Q: How do I change the cube rotation speed?**
A: Look in Application.cpp → Update() function, change the `dt * 20.0f` value.

---

## 📊 Documentation Statistics

- **Code Comments**: ~500 lines of detailed comments across all files
- **Documentation Files**: 4 markdown files with ~2000 lines total
- **Diagrams**: 10 ASCII architecture diagrams
- **Examples**: 20+ code examples in quick reference
- **Coverage**: All public APIs, key algorithms, and shader logic

---

## 🎓 Learning Resources in This Project

### If You Want to Learn OpenGL
- Read shader comments to understand vertex/fragment processing
- Look at Mesh.h to understand VAO/VBO/EBO
- See Renderer.cpp to understand how transforms work

### If You Want to Learn Scene Graphs
- Read Scenemap.h thoroughly
- Look at ARCHITECTURE_DIAGRAMS.md for hierarchy visualization
- Study Application.cpp to see how hierarchy is built

### If You Want to Learn Lighting
- Read shader.frag comments carefully
- Study ARCHITECTURE_DIAGRAMS.md "Lighting Model" diagram
- Modify lighting equations to experiment

### If You Want to Learn Graphics Pipelines
- Follow the data flow in ARCHITECTURE_DIAGRAMS.md
- Trace through Renderer.cpp step by step
- Read Application.cpp render loop

---

## 💡 Tips for Success

1. **Start with QUICK_REFERENCE.md** - Get oriented quickly
2. **Use ARCHITECTURE_DIAGRAMS.md** - Visualize before diving into code
3. **Read header files first** - Understand the interface before implementation
4. **Follow the data flow** - Understand how data moves through the system
5. **Try small changes** - Modify values and see what happens
6. **Read shader comments** - This is where most "magic" happens

---

## 📞 Issues or Questions?

If something is unclear:
1. Check if it's in QUICK_REFERENCE.md
2. Look for diagrams in ARCHITECTURE_DIAGRAMS.md
3. Read the detailed comments in the source files
4. Consult ARCHITECTURE_GUIDE.md for system-level explanations

Good luck with your graphics engine! 🎮

---

Last Updated: March 27, 2026
Total Documentation: 4 files + extensive inline comments
Status: Complete and ready for development ✓


