//
// Created by Zyb3r on 27/03/2026.
//

#include "Panel.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include "GameObject.h"
#include "Helper.h"
#include "ResourceManager.h"
#include "Scenemap.h"
Scenemap* Panel::scene = nullptr;
std::shared_ptr<std::vector<LogMessage>> Panel::debugList = std::make_shared<std::vector<LogMessage>>();
Application*Panel::app = nullptr;

// ========== PANEL CLASS ==========
// This is responsbile for rendering all the windows and panels in the engine (scene hierarchy, debug console, etc)
void Panel::Render() {

    // 1. Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // This creates a simple window with the average frame time and FPS, which is useful for performance monitoring
    ImGui::Begin("Engine Tools");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // 2. Render the scene hierarchy panel and debug console
    HirearchicalPanel();
    DebugConsole();

    // 4. Final ImGui Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Initialize: Sets up ImGui context and initializes it for use with GLFW and OpenGL
void Panel::Initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

// DebugConsole: Renders a console window that displays log messages with different colors based on severity (INFO, WARNING, ERROR)
void Panel::DebugConsole() {
    ImGui::Begin("Debug Console");
    if (debugList) {
        for (const auto& entry : *debugList) {
            ImVec4 color;

            // Assign colors based on severity
            switch (entry.level) {
                case LogLevel::WARNING: color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f); break; // Yellow
                case LogLevel::ERROR:     color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); break; // Red
                default:                color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break; // White
            }

            ImGui::TextColored(color, "%s", entry.message.c_str());
        }

        ImGui::SetScrollHereY(1.0f);
        if (ImGui::Button("Clear")) {
            ClearLogs();
        }
    }
    ImGui::End();
}

// ClearLogs: Clears all log messages from the debug console
void Panel::ClearLogs() {
    if (debugList) {
        debugList->clear();
    }
}

// AddLog: Adds a new log message to the debug console with the specified severity level
void Panel::AddLog(const std::string& message, LogLevel level) {
    if (!debugList) {
        debugList = std::make_shared<std::vector<LogMessage>>();
    }
    debugList->push_back({ level, message });
}

void Panel::HirearchicalPanel() {
    ImGui::Begin("Scene Hierarchy");
    if (ImGui::BeginPopupContextItem("Menu")) {
        if (ImGui::MenuItem("Create Cube")) {
            if (app) app->CreateCube(*scene->AddRootNode());
        }
        if (ImGui::MenuItem("Create Root Node")) {
            if (app) GameObject::Create("New Node", scene);
        }

        ImGui::EndPopup();
    }

    // Ensure the scene pointer is valid before accessing
    if (Panel::scene == nullptr) {
        ImGui::End();
        return;
    }

    auto& rootNodes = Panel::scene->GetRootNodes();

    for (int j = 0; j < rootNodes.size(); j++) {
        std::shared_ptr<SceneNode> node = rootNodes[j];
        std::string rootLabel = node->gameObject ? node->gameObject->name : "Root Node " + std::to_string(j);

        // We wrap the Target logic around the Root Node so you can drop things ONTO it
        bool nodeOpen = ImGui::TreeNode(rootLabel.c_str());

        if (ImGui::BeginPopupContextItem("RootNodeMenu")) {
            if (ImGui::MenuItem("Delete")) {
                DeleteNode(node);
            }
            if (ImGui::MenuItem("Create Child Cube")) {
                app->CreateCube(*node);

            }
            if (ImGui::MenuItem("Create Point Light")) {
                GameObject::CreatePointLight("New Point Light", scene, node);

            }

            ImGui::EndPopup();
        }
        // --- DRAG TARGET: Drop a node onto this Root Node to make it a child ---
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NODE_MOVE")) {
                // Retrieve the shared_ptr from the payload
                std::shared_ptr<SceneNode> draggedNode = *(std::shared_ptr<SceneNode>*)payload->Data;

                // Safety: Don't drop a node onto itself
                if (draggedNode != node) {
                    // Re-parent the dragged node to this root node
                    if (draggedNode->parent) {
                        draggedNode->parent->RemoveChildNode(draggedNode);
                    } else {
                        // If the node had no parent, it was a root node, so remove it from the root list
                        Panel::scene->RemoveRootNode(draggedNode);
                    }

                    node->AddChildNode(draggedNode);


                    AddLog("Re-parented " + (draggedNode->gameObject ? draggedNode->gameObject->name : "Node") + " to " + rootLabel, LogLevel::INFO);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (nodeOpen) {
            // Transform Sliders for the Root Node
            ImGui::DragFloat3("Position", &node->GetPosition().x, 0.1f);
            ImGui::DragFloat3("Rotation", &node->GetRotation().x, 0.1f);
            ImGui::DragFloat3("Scale", &node->GetScale().x, 0.1f);

            auto& children = node->GetChildNodes();
            for (int i = 0; i < children.size(); i++) {
                auto& child = children[i];
                if (!child || !child->gameObject) continue;
                ImGui::PushID(child.get()); // Ensure unique ID for ImGui elements
                if (child->gameObject != nullptr) {
                    if (ImGui::TreeNode(child->gameObject->name.c_str())) {

                        if (ImGui::BeginPopupContextItem("ChildMenu")) {
                            if (ImGui::MenuItem("Delete")) {
                                DeleteNode(child);
                            }

                            ImGui::EndPopup();
                        }

                        // --- DRAG SOURCE: Allow this child to be picked up ---
                        if (ImGui::BeginDragDropSource()) {
                            ImGui::SetDragDropPayload("NODE_MOVE", &child, sizeof(std::shared_ptr<SceneNode>));
                            ImGui::Text("Moving %s", child->gameObject->name.c_str());
                            ImGui::EndDragDropSource();
                        }

                        // Child Transform Sliders
                        ImGui::DragFloat3("Position", &child->GetPosition().x, 0.1f);
                        ImGui::DragFloat3("Rotation", &child->GetRotation().x, 0.1f);
                        ImGui::DragFloat3("Scale", &child->GetScale().x, 0.1f);
                        ImGui::TreePop();
                    }
                }
                ImGui::PopID();
            }
                ImGui::TreePop();
        }
    }

    ImGui::End();
}

void Panel::DeleteNode(std::shared_ptr<SceneNode> node) {
    node->Delete(node, *scene);
     AddLog("Deleted " + (node->gameObject ? node->gameObject->name : "Node"), LogLevel::WARNING);
}
