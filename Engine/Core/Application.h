#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

// Full includes to prevent "incomplete type" errors
#include "../Renderer/Renderer.h"
#include "../Scene/Light.h"
#include "../Scene/Scenemap.h" // Add this
#include "../Scene/Camera.h"

class Application {
public:
    Application(unsigned int width, unsigned int height);
    ~Application();

    bool Init();
    void Run();

    // Static callbacks required for GLFW
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    GLFWwindow* window;
    unsigned int screenWidth, screenHeight;

    // Engine Systems
    Renderer renderer;
    Scenemap scene;
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    Camera camera;
    std::vector<Light> lights;

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif