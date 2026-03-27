//
// Created by Zyb3r on 13/02/2026.
//
// Renderer.h - Handles rendering the scene graph to the screen
// Takes the scene hierarchy and renders each mesh with proper transforms
// Applies lighting, textures, and shaders to each mesh
// LINKS TO: Application (calls Draw each frame), Scenemap (traverses scene), Camera (view/projection)

#ifndef GRAPHICSENGINE_RENDERER_H
#define GRAPHICSENGINE_RENDERER_H
#include "../Scene/Camera.h"
#include "../Scene/Scenemap.h"
#include "../Scene/Light.h"


class Renderer {
public:
    // ========== MAIN RENDERING FUNCTION ==========
    // Draw: Main entry point - renders entire scene
    // Parameters:
    //   scene (Scenemap&) - the scene graph containing all nodes/meshes
    //   camera (Camera&) - the camera defining view and projection matrices
    //   lights (std::vector<Light>&) - array of light sources affecting the scene
    // Flow: Called by Application::Render() each frame
    //       1. Iterates through all root nodes
    //       2. Calls RenderNode for each with identity matrix
    //       3. RenderNode recursively renders children
    void Draw(Scenemap &scene, Camera &camera, std::vector<Light> &lights);

private:
    // ========== RECURSIVE RENDERING FUNCTION ==========
    // RenderNode: Recursively renders a node and its children
    // This is the core of the scene graph rendering algorithm
    //
    // Parameters:
    //   node (std::shared_ptr<SceneNode>&) - the current node being rendered
    //   parentTransform (glm::mat4&) - the accumulated transform from all parent nodes
    //   camera (Camera&) - the camera for view/projection matrices
    //   lights (std::vector<Light>&) - light sources affecting all meshes
    //
    // Algorithm:
    //   1. Calculate world transform: parentTransform * node's local transform
    //   2. For each mesh in this node:
    //      a. Bind its shader
    //      b. Set all uniform variables (model, view, projection, lights, material, textures)
    //      c. Calculate final mesh transform: world transform * mesh's local transform
    //      d. Call mesh.Draw() to render to GPU
    //   3. For each child node:
    //      a. Recursively call RenderNode, passing the world transform as parent transform
    //
    // Flow: Called by Draw() for root nodes, then recursively for children
    void RenderNode(const std::shared_ptr<SceneNode> &node, const glm::mat4 &parentTransform, Camera &camera,
                    std::vector<Light> &lights);
};


#endif //GRAPHICSENGINE_RENDERER_H
