//
// Created by Zyb3r on 27/03/2026.
//

#ifndef GRAPHICSENGINE_PANEL_H
#define GRAPHICSENGINE_PANEL_H
#include <glad/glad.h>
#include <memory>
#include <GLFW/glfw3.h>

#include "Scenemap.h"


class Scenemap;

class Panel {
public:
    static void Render();
    static void Initialize(GLFWwindow* window);
    static void HirearchicalPanel();
    static Scenemap scene;

};


#endif //GRAPHICSENGINE_PANEL_H