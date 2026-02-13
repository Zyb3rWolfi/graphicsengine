cmake_minimum_required(VERSION 4.1)
project(graphicsengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(graphicsengine main.cpp
        vertex.cpp
        Vertex.h
        Shader.cpp
        Shader.h
        Texture.cpp
        Texture.h
        Mesh.cpp
        Mesh.h
        ShapeFactory.cpp
        ShapeFactory.h
        Camera.cpp
        Camera.h
        Scenemap.cpp
        Scenemap.h
        Renderer.cpp
        Renderer.h
)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glfw)
find_package(glad CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glad::glad)
find_package(glm CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(graphicsengine PRIVATE ${Stb_INCLUDE_DIR})