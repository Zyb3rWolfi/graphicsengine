#include "Application.h"
#include <iostream>

#include "Input.h"
#include "ResourceManager.h"
#include "../Utility/EngineTime.h"
#include "stb_image.h"
#include "../Utility/ShapeFactory.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Texture.h"

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

    lights = {
        Light(glm::vec3(-2.0f, 1.0f, -1.0f)),  // Light positioned to the left and above
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

    // Load shader programs from disk
    // Each shader consists of vertex and fragment shader source files
    // Shaders are registered by name for later retrieval and use
    ResourceManager::LoadShader("Shaders/shader.vert", "Shaders/shader.frag", "MainShader");     // Main lighting shader
    ResourceManager::LoadShader("Shaders/sourceShader.vert", "Shaders/sourceShader.frag", "LightShader");  // Light source shader

    // Create two cube meshes using ShapeFactory
    // These are raw GPU geometry - they're moved into the scene graph next
    Mesh cubeMain = factory.CreateCube();
    Mesh cubeLight = factory.CreateCube();

    // ========== CONFIGURE MAIN CUBE ==========
    // Set material properties for the first cube
    cubeMain.objectColor = glm::vec3(0.0f, 1.0f, 0.0f);  // Green base color
    cubeMain.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);   // White light (unused mostly)
    cubeMain.shininess = 32.0f;                          // Moderate shininess
    cubeMain.ambient = glm::vec3(0.2f, 0.2f, 0.2f);      // Low ambient = dark in shadows
    cubeMain.diffuse = glm::vec3(0.8f, 0.9f, 0.8f);      // Light diffuse = bright scattered light
    cubeMain.specular = glm::vec3(1.0f, 1.0f, 1.0f);     // Full white specular = shiny highlights

    // ========== LOAD TEXTURES ==========
    // Load texture files into ResourceManager for use on meshes
    ResourceManager::LoadTexture("Images/wall/default.jpg", "wall_map");    // Diffuse/color texture
    ResourceManager::LoadTexture("Images/wall/normal.jpg", "wall_normal");  // Normal map for surface detail

    // ========== BUILD SCENE GRAPH ==========
    // Create root node at origin (0, 0, 0)
    // The scene graph organizes meshes hierarchically
    auto root = scene.AddRootNode(glm::vec3(0.0f));

    root->AddChildMesh(std::move(cubeMain),
        ResourceManager::GetTexture("wall_map"),    // Use the wood texture
        nullptr,                                     // No specular map
        nullptr,                                     // No emission
        ResourceManager::GetTexture("wall_normal"),  // Use normal map for surface detail
        ResourceManager::GetShader("MainShader"),    // Use main lighting shader
        glm::vec3(2.0f, 0.0f, 0.0f));                // Position at (2, 0, 0) in root space

    root->AddChildMesh(std::move(cubeLight),
        nullptr,  // No texture
        nullptr,  // No specular
        nullptr,  // No emission
        nullptr,  // No normal map
        ResourceManager::GetShader("LightShader"),   // Use simple light shader
        glm::vec3(-2.0f, 1.0f, -1.0f));              // Position at (-2, 1, -1)

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
}

// Global-to-Class bridge for GLFW callbacks
void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
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