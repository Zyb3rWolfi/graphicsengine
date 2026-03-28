//
// Created by Zyb3r on 27/03/2026.
//

#ifndef GRAPHICSENGINE_PANEL_H
#define GRAPHICSENGINE_PANEL_H
#include <glad/glad.h>
#include <memory>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Scenemap.h"


class Scenemap;

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

struct LogMessage {
    LogLevel level;
    std::string message;
};

class Panel {
public:
    static void Render();
    static void Initialize(GLFWwindow* window);
    static void HirearchicalPanel();
    static void DebugConsole();
    static Application *app;
    static Scenemap* scene;
    static std::shared_ptr<std::vector<LogMessage>> debugList;
    static void AddLog(const std::string& message, LogLevel level = LogLevel::INFO);
    static void ClearLogs();
    static void DeleteNode(std::shared_ptr<SceneNode> node);
};


#endif //GRAPHICSENGINE_PANEL_H