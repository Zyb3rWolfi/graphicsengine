//
// Created by Zyb3r on 18/03/2026.
//

#include "Input.h"

GLFWwindow* Input::s_Window = nullptr;
std::unordered_map<std::string, std::vector<int>> Input::s_ActionMaps;

void Input::Initialize(GLFWwindow* window) {
    s_Window = window;
}

void Input::MapAction(const std::string& actionName, int glfwKey) {
    s_ActionMaps[actionName].push_back(glfwKey);
}

bool Input::IsActionActive(const std::string& actionName) {
    if (s_ActionMaps.find(actionName) == s_ActionMaps.end()) return false;

    // If any key assigned to this action is pressed, return true
    for (int key : s_ActionMaps[actionName]) {
        if (IsKeyDown(key)) return true;
    }
    return false;
}

bool Input::IsKeyDown(int key) {
    if (s_Window == nullptr) {
        // Log an error or simply return false to avoid a crash
        return false;
    }
    int state = glfwGetKey(s_Window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 Input::GetMousePosition() {
    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);
    return glm::vec2((float)x, (float)y);
}