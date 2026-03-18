#include "Application.h"
#include <iostream>

#include "../Utility/ShapeFactory.h"

Application::Application(unsigned int width, unsigned int height)
    : screenWidth(width), screenHeight(height), camera(glm::vec3(0.0f, 0.0f, 3.0f)) {

    // Setup lights from your main.cpp
    lights = {
        Light(glm::vec3(-2.0f, 1.0f, -1.0f)),
        Light(glm::vec3(4.0f, 1.0f, -1.0f))
    };
}

bool Application::Init() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Zyb3r Engine", NULL, NULL);
    if (!window) return false;

    glfwMakeContextCurrent(window);

    // Register this class instance with the window so callbacks can access it
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Application::FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, Application::MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glEnable(GL_DEPTH_TEST);
    return true;
}

void Application::Run() {
    ShapeFactory factory;
    // Loading resources moved from your main.cpp
    Shader shader("Shaders/shader.vert", "Shaders/shader.frag");
    Shader shader2("Shaders/sourceShader.vert", "Shaders/sourceShader.frag");

    Mesh cubeMain = factory.CreateCube();
    Mesh cubeLight = factory.CreateCube();

    Texture diff("Images/container2.png");
    Texture spec("Images/container2_specular.png");

    auto root = scene.AddRootNode(glm::vec3(0.0f));
    root->AddChildMesh(&cubeMain, &diff, &spec, nullptr, &shader, glm::vec3(2.0f, 0.0f, 0.0f));
    root->AddChildMesh(&cubeLight, nullptr, nullptr, nullptr, &shader2, glm::vec3(-2.0f, 1.0f, -1.0f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput();
        Update(deltaTime);
        Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::ProcessInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Keeping movement simple as requested
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
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
}