cmake_minimum_required(VERSION 4.1)
project(graphicsengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(graphicsengine main.cpp
        Engine/Renderer/vertex.cpp
        Engine/Renderer/Vertex.h
        Engine/Renderer/Shader.cpp
        Engine/Renderer/Shader.h
        Engine/Renderer/Texture.cpp
        Engine/Renderer/Texture.h
        Engine/Renderer/Mesh.cpp
        Engine/Renderer/Mesh.h
        Engine/Utility/ShapeFactory.cpp
        Engine/Utility/ShapeFactory.h
        Engine/Scene/Camera.cpp
        Engine/Scene/Camera.h
        Engine/Scene/Scenemap.cpp
        Engine/Scene/Scenemap.h
        Engine/Renderer/Renderer.cpp
        Engine/Renderer/Renderer.h
        Engine/Scene/Light.h
        Engine/Core/Application.cpp
        Engine/Core/Application.h
)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glfw)
find_package(glad CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glad::glad)
find_package(glm CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(graphicsengine PRIVATE ${Stb_INCLUDE_DIR})