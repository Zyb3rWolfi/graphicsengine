#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "GameObject.h"
#include "Input.h"
#include "ResourceManager.h"
#include "../Utility/EngineTime.h"
#include "../Utility/ShapeFactory.h"
#include "../Renderer/Mesh.h"
#include "Engine/UI/Panel.h"

// ========== APPLICATION CLASS ==========
// Application: Main engine class that manages the entire graphics engine
// Responsible for:
//   1. Window creation and GLFW/GLAD initialization
//   2. Input handling and keyboard callbacks
//   3. Scene setup and resource loading
//   4. Main render loop
// LINKS TO: All systems (renderer, scene, camera, input)

// ========== CONSTRUCTOR ==========
// Initializes application with given screen dimensions
// Parameters:
//   width - window width in pixels (e.g., 800)
//   height - window height in pixels (e.g., 600)
// Sets up:
//   - Screen dimensions
//   - Camera with default position (0, 0, 3) - looking at origin
//   - Light sources that affect the scene
Application::Application(unsigned int width, unsigned int height)
    : screenWidth(width), screenHeight(height), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {

    // ========== LIGHTING SETUP ==========
    // The engine supports three types of lights: Directional, Point, and Spot

    // Directional Light (like the sun - comes from infinity)
    Light dirLight;
    dirLight.LightType = LightType::DIRECTIONAL;
    dirLight.direction = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));  // Direction light travels FROM
    dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    dirLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Point Light (like a light bulb - emits from a position)
    Light pointLight;
    pointLight.LightType = LightType::POINT;
    pointLight.position = glm::vec3(-2.0f, 1.0f, -1.0f);
    pointLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pointLight.diffuse = glm::vec3(0.8f, 0.7f, 0.5f);
    pointLight.specular = glm::vec3(1.0f, 1.0f, 0.8f);
    // Attenuation for ~50 unit range
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;

    // Add lights to the scene
    lights = {
        //dirLight,    // Directional light acts like sun/moon
        pointLight   // Point light acts like a torch or bulb
    };
}

// ========== INITIALIZATION FUNCTION ==========
// Init: Sets up GLFW window, OpenGL context, and input system
// Flow: Called once from main() before Run()
// Returns: true if successful, false if initialization failed
bool Application::Init() {
    // Initialize GLFW library
    if (!glfwInit()) return false;
    // Request OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    // Parameters: width, height, title, monitor (NULL = windowed), share context (NULL)
    window = glfwCreateWindow(screenWidth, screenHeight, "Zyb3r Engine", NULL, NULL);
    if (!window) return false;

    // Initialize input system with the newly created window
    Input::Initialize(window);

    // Make the OpenGL context current for this thread
    glfwMakeContextCurrent(window);

    // ========== INPUT MAPPING ==========
    // Map keyboard keys to action names so we can check actions rather than specific keys
    Input::MapAction("MoveForward", GLFW_KEY_W);
    Input::MapAction("MoveForward", GLFW_KEY_UP);     // Multiple keys for one action
    Input::MapAction("MoveLeft",    GLFW_KEY_A);
    Input::MapAction("MoveRight",   GLFW_KEY_D);
    Input::MapAction("MoveBack",    GLFW_KEY_S);
    Input::MapAction("Debug",       GLFW_KEY_F1);
    Input::MapAction("Quit",        GLFW_KEY_ESCAPE);

    // Register this Application instance with GLFW callbacks
    // This allows static callback functions to access the Application instance
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Application::FramebufferSizeCallback);  // Window resize
    glfwSetCursorPosCallback(window, Application::MouseCallback);                   // Mouse movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                   // Hide cursor

    // Load OpenGL function pointers using GLAD
    // This initializes all OpenGL function pointers for the current OpenGL version
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    Panel::Initialize(window);
    // Enable depth testing for 3D rendering
    // This ensures closer objects render in front of farther ones
    glEnable(GL_DEPTH_TEST);
    return true;
}

// ========== MAIN GAME LOOP ==========
// Run: The main application loop - runs until window closes
// Flow: Called once from main() after Init()
// Sequence each frame:
//   1. ProcessInput() - handle keyboard/mouse input
//   2. Update() - update game logic (animations, transforms)
//   3. Render() - render the scene to the screen
//   4. glfwSwapBuffers() - display the rendered frame
//   5. glfwPollEvents() - process OS events
void Application::Run() {
    EngineTime::Update();

    // ========== SCENE SETUP ==========
    ShapeFactory factory;

    ResourceManager::LoadShader("Shaders/shader.vert", "Shaders/shader.frag", "MainShader");     // Main lighting shader
    ResourceManager::LoadShader("Shaders/sourceShader.vert", "Shaders/sourceShader.frag", "LightShader");  // Light source shaders
    ResourceManager::LoadTexture("Images/wall/default.jpg", "wall_map");    // Diffuse/color texture
    ResourceManager::LoadTexture("Images/wall/normal.jpg", "wall_normal");  // Normal map for surface detail

    auto root = scene.AddRootNode(glm::vec3(0.0f));


    for (int i = 0; i < 5; i++) {
        auto wall = GameObject::CreateCube("wall" + std::to_string(i));
        wall->SetTexture(ResourceManager::GetTexture("wall_map"));
        wall->SetNormalTexture(ResourceManager::GetTexture("wall_normal"));
        wall->SetShader(ResourceManager::GetShader("MainShader"));

        wall->SetPosition(glm::vec3(i * 1.0f, 0.0f, -5.0f));
        root->AddChildNode(wall->node);
        gameObjects.push_back(wall);

    }

    // ========== MAIN RENDER LOOP ==========
    // Runs until window close is requested
    while (!glfwWindowShouldClose(window)) {
        // Get elapsed time since last frame
        EngineTime::Update();
        float dt = EngineTime::GetDeltaTime();

        // Handle keyboard/mouse input
        ProcessInput(dt);

        // Update game state (animations, positions, etc.)
        Update(dt);

        // Render the scene
        Render();

        // Display the rendered frame (double buffering: swap front and back buffers)
        glfwSwapBuffers(window);

        // Process OS events (keyboard, mouse, window messages)
        glfwPollEvents();
    }
}

// ========== INPUT PROCESSING ==========
// ProcessInput: Handles user input (keyboard/mouse)
// Parameters:
//   dt - delta time since last frame (for frame-rate independent movement)
// Flow: Called once per frame before rendering
void Application::ProcessInput(float dt) {
    // Check if quit action is active (ESC key)
    if (Input::IsActionActive("Quit")) {
        glfwSetWindowShouldClose(window, true);
    }

    if (Input::IsActionActive("MoveForward")) {
        camera.ProcessKeyboard(FORWARD, dt);
    }
    if (Input::IsActionActive("MoveLeft")) {
        camera.ProcessKeyboard(LEFT, dt);
    }
    if (Input::IsActionActive("MoveRight")) {
        camera.ProcessKeyboard(RIGHT, dt);
    }
    if (Input::IsActionActive("MoveBack")) {
        camera.ProcessKeyboard(BACKWARD, dt);
    }

    static bool wasDebugPressed = false;
    bool isdebugPressed = Input::IsActionActive("Debug");
    // Debug mode toggle
    if (isdebugPressed && !wasDebugPressed) {
        // Unlock the mouse cursor when F1 is pressed for debugging and lock it again when released
        static bool debugMode = false;
        debugMode = !debugMode;
        if (debugMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    wasDebugPressed = isdebugPressed;


}

void Application::Update(float dt) {
    // Rotation logic from your loop
    for (auto& root : scene.GetRootNodes()) {
        root->Rotate(glm::vec3(0.0f, dt * 20.0f, 0.0f));
    }
}

void Application::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.SetProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 50.0f));
    renderer.Draw(scene, camera, lights);
    // 2. Start ImGui Frame
    Panel::scene = scene; // Pass the current scene to the panel for rendering
    Panel::Render();

}

// Global-to-Class bridge for GLFW callbacks
void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        // Simple logic from main.cpp
        static float lastX = 400, lastY = 300;
        static bool firstMouse = true;
        if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
        app->camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);
        lastX = xpos; lastY = ypos;
    }
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Application::~Application() {
    glfwTerminate();
    ResourceManager::Clean();
}