#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Scenemap.h"
#include "stb_image.h"

#include "Shader.h"
#include "ShapeFactory.h"
#include "Texture.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
Renderer renderer;

// Settings for the screen size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera Settings
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse States
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

std::vector<Light> lights = {
    Light(glm::vec3(-2.0f, 1.0f, -1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f)),
    Light(glm::vec3(4.0f, 1.0f, -1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f))
};

void mouse_callback(GLFWwindow * window, double xpos, double ypos);

int main() {

    // Initilize GLFW & Version
    glfwInit();

    ShapeFactory factory;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    // Setting up the input mode
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwMakeContextCurrent(window); // Make the window's context current
    // Registering the callback function for when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader = Shader("Shaders/shader.vert", "Shaders/shader.frag");
    Shader shader2 = Shader("Shaders/sourceShader.vert", "Shaders/sourceShader.frag");
    // Create meshes (they need to persist in memory)
    Mesh cube1 = factory.CreateCube();
    Mesh cube2 = factory.CreateCube();
    Mesh cube3 = factory.CreateCube();
    //cube1.objectColor = glm::vec3(0.3f, 0.3f, 0.3f);
    cube1.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    cube2.objectColor = glm::vec3(0.0f, 1.0f, 0.0f);
    cube2.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    cube2.shininess = 32.0f;
    cube2.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    cube2.diffuse = glm::vec3(0.8f, 0.9f, 0.8f);
    cube2.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    cube1.shininess = 32.0f;
    cube1.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    cube1.diffuse = glm::vec3(0.8f, 0.9f, 0.8f);
    cube1.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Cube 3 is a light source, so it has a different shader and no texture
    cube3.objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
    cube3.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    cube3.shininess = 32.0f;
    cube3.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    cube3.diffuse = glm::vec3(0.8f, 0.9f, 0.8f);
    cube3.specular = glm::vec3(1.0f, 1.0f, 1.0f);


    // Create the scene
    Scenemap scene;
    Texture text = Texture("Images/container.jpg");
    Texture text2 = Texture("Images/container2.png");
    Texture specularMap = Texture("Images/container2_specular.png");
    Texture emissionMap = Texture("Images/Box/emission.jpg");

    // Create a root node at world position (0, 0, 0)
    auto rootNode1 = scene.AddRootNode(glm::vec3(0.0f, 0.0f, 0.0f));
    // Add child meshes with local transforms relative to the root node
    // cube1 uses shader (the main shader), cube2 uses shader2 (sourceShader)
    rootNode1->AddChildMesh(&cube2, &text2, &specularMap, nullptr,&shader, glm::vec3(2.0f, 0.0f, 0.0f));
    rootNode1->AddChildMesh(&cube3, nullptr,nullptr,nullptr ,&shader2, glm::vec3(-2.0f, 1.0f, -1.0f));
    rootNode1->AddChildMesh(&cube3, nullptr,nullptr,nullptr ,&shader2, glm::vec3(4.0f, 1.0f, -1.0f));


    while (!glfwWindowShouldClose(window)) {
        glm::vec3 lightPos = glm::vec3(-2.0f, 1.0f, -1.0f);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Animate the first root node (rotate it in world space)
        rootNode1->Rotate(glm::vec3(0.0f, deltaTime * 20.0f, 0.0f));

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 50.0f);
        camera.SetProjectionMatrix(projection);

        // Render all root nodes and their children (each mesh uses its own shader)
        renderer.Draw(scene, camera, lights);

        // Double buffering so no screen tearing
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();


    return 0;
}
void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}