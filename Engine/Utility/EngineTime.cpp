//
// Created by Zyb3r on 18/03/2026.
//

#include "EngineTime.h"
#include <GLFW/glfw3.h>

// Initializing static members
float EngineTime::m_LastFrame = 0.0f;
float EngineTime::m_CurrentFrame = 0.0f;
float EngineTime::m_DeltaTime = 0.0f;

void EngineTime::Update() {
    // glfwGetTime returns double, cast to float for engine use
    m_CurrentFrame = static_cast<float>(glfwGetTime());
    m_DeltaTime = m_CurrentFrame - m_LastFrame;
    m_LastFrame = m_CurrentFrame;
}