
#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>

class Input {
public:
    // Setup and Polling
    static void Initialize(GLFWwindow* window);

    // Action Mapping
    static void MapAction(const std::string& actionName, int glfwKey);
    static bool IsActionActive(const std::string& actionName);

    // Direct Key Queries
    static bool IsKeyDown(int key);
    static bool IsKeyPressed(int key); // Only true for the first frame

    // Mouse Queries
    static glm::vec2 GetMousePosition();

private:
    static GLFWwindow* s_Window;
    static std::unordered_map<std::string, std::vector<int>> s_ActionMaps;
};




#endif //INPUT_H
