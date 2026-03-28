cmake_minimum_required(VERSION 3.29.6)
project(graphicsengine)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.90.4
)

# Use Populate instead of MakeAvailable for ImGui
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# 1. Properly list all files
add_executable(graphicsengine
        main.cpp
        # ImGui Core (Now using the populated path)
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        # ImGui Backends
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

        # Engine Core
        Engine/Core/Application.cpp
        Engine/Core/Application.h

        # Engine Renderer
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

        # Engine Scene & Objects
        Engine/Scene/Camera.cpp
        Engine/Scene/Camera.h
        Engine/Scene/Scenemap.cpp
        Engine/Scene/Scenemap.h
        Engine/Scene/Light.h
        Engine/Scene/GameObject.cpp
        Engine/Scene/GameObject.h

        # Engine Utility (Fixed: removed duplicate EngineTime.cpp)
        Engine/Utility/ShapeFactory.cpp
        Engine/Utility/ShapeFactory.h
        Engine/Utility/stb_impl.cpp
        Engine/Utility/Input.cpp
        Engine/Utility/Input.h
        Engine/Utility/EngineTime.cpp
        Engine/Utility/EngineTime.h
        Engine/Utility/ResourceManager.cpp
        Engine/Utility/ResourceManager.h
        Engine/UI/Panel.cpp
        Engine/UI/Panel.h
        Engine/Utility/Helper.cpp
        Engine/Utility/Helper.h
)

# 2. Add include paths
target_include_directories(graphicsengine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Core
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Scene
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Utility
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# 3. Linking
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(graphicsengine PRIVATE glm::glm)

# vcpkg usually needs explicit OpenGL linking for backends
find_package(OpenGL REQUIRED)
target_link_libraries(graphicsengine PRIVATE OpenGL::GL)