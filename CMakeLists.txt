cmake_minimum_required(VERSION 3.29.6)
project(graphicsengine)

set(CMAKE_CXX_STANDARD 20)

# 1. Properly list all files once
add_executable(graphicsengine
        main.cpp
        Engine/Core/Application.cpp
        Engine/Core/Application.h
        Engine/Renderer/Renderer.cpp
        Engine/Renderer/Renderer.h
        Engine/Renderer/Mesh.cpp
        Engine/Renderer/Mesh.h
        Engine/Renderer/Shader.cpp
        Engine/Renderer/Shader.h
        Engine/Renderer/Texture.cpp
        Engine/Renderer/Texture.h
        Engine/Renderer/vertex.cpp
        Engine/Renderer/Vertex.h
        Engine/Scene/Camera.cpp
        Engine/Scene/Camera.h
        Engine/Scene/Scenemap.cpp
        Engine/Scene/Scenemap.h
        Engine/Scene/Light.h
        Engine/Utility/ShapeFactory.cpp
        Engine/Utility/ShapeFactory.h
        Engine/Utility/stb_impl.cpp
        Engine/Utility/Input.cpp
        Engine/Utility/Input.h
        Engine/Utility/EngineTime.cpp
        Engine/Utility/EngineTime.h
        Engine/Utility/EngineTime.cpp

)

# 2. Add your Engine subdirectories to the include path
# This allows "#include "stb_image.h"" to find the local file from anywhere
target_include_directories(graphicsengine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Scene
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Utility
)

# 3. Standard Packages
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(graphicsengine PRIVATE ${Stb_INCLUDE_DIR})

# REMOVE find_package(Stb) - you are compiling it manually now!