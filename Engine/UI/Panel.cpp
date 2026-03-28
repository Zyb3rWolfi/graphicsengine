//
// Created by Zyb3r on 27/03/2026.
//

#include "Panel.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GameObject.h"
#include "Scenemap.h"
Scenemap Panel::scene;

void Panel::Render() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 3. Create your Windows
    ImGui::Begin("Engine Tools");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    HirearchicalPanel();

    // 4. Final ImGui Render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Panel::Initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Panel::HirearchicalPanel() {
    std::shared_ptr<SceneNode> scene = std::make_shared<SceneNode>();
    scene = Panel::scene.GetRootNodes()[0]; // Get the first root node for demonstration

    for (int i=0; i < scene->GetChildNodes().size(); i++) {
        std::shared_ptr<SceneNode> child = scene->GetChildNodes()[i];
        if (ImGui::TreeNode(child->gameObject->name.c_str())) {
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui::Text("Position: (%.2f, %.2f, %.2f)", child->GetPosition().x, child->GetPosition().y, child->GetPosition().z);
            ImGui::DragFloat3("Position", &child->GetPosition().x, 0.1f);
            ImGui::DragFloat3("Rotation", &child->SetRotation(), 0.1f);
            ImGui::DragFloat3("Scale", &child->GetScale().x, 0.1f);
            ImGui::TreePop();
        }
    }



}