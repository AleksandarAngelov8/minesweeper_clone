#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFunctionality.h"
#include "Scene/MultiplayerScenes/Arena.h"
#include "Game/Multiplayer/PlayerCharacter.h"

void ImGuiFunctionality::ImGuiKeyButtonCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window,key,scancode,action,mods);
}
void ImGuiFunctionality::ImGuiMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}
bool ImGuiFunctionality::ImGuiWantsMouse()
{
    return ImGui::GetIO().WantCaptureMouse;
}
void ImGuiFunctionality::Init()
{
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(ButtonFunctionality::globalWindow, true);
    ImGui::StyleColorsDark();
    ImGui::CaptureMouseFromApp();
}
void ImGuiFunctionality::Update(float xpos, float ypos)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("%f.2 - %f.2", xpos, ypos);

    if (ButtonFunctionality::currentSceneId == "arena") {
        std::shared_ptr<Arena> arenaScene = std::dynamic_pointer_cast<Arena>(ButtonFunctionality::getCurrentSceneObject());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Character: %.2f\t%.2f", arenaScene->character->GetTranslation().x, arenaScene->character->GetTranslation().y);
        ImGui::Text("Enemy: %.2f\t%.2f", arenaScene->enemy->GetTranslation().x, arenaScene->enemy->GetTranslation().y);
        ImGui::Text("Camera: %.2f\t%.2f", Mesh::cameraTranslate.x, Mesh::cameraTranslate.y);
        ImGui::Text("%d", arenaScene->elapsedTime.count());
        ImGui::Text("Character milk: %d", arenaScene->character->GetMissiles().size());
        ImGui::Text("Enemy milk: %d", arenaScene->enemy->GetMissiles().size());
        //ImGui::Text("Trigger: %d", std::dynamic_pointer_cast<PlayerCharacter>(arenaScene->character)->triggerChangeAnimationSprite);
        if (arenaScene->character->GetMoving(0)) ImGui::Text("Moving Up");
        if (arenaScene->character->GetMoving(1)) ImGui::Text("Moving Right");
        if (arenaScene->character->GetMoving(2)) ImGui::Text("Moving Down");
        if (arenaScene->character->GetMoving(3)) ImGui::Text("Moving Left");
    }
}
void ImGuiFunctionality::Draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiFunctionality::ShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
std::string ImGuiFunctionality::cellData = "";
char ImGuiFunctionality::textureName[128] = ""; // Assuming max texture name length is 128
float ImGuiFunctionality::maxHP = 30;
float ImGuiFunctionality::AD = 5;
unsigned int ImGuiFunctionality::level = 1;
char ImGuiFunctionality::name[128] = "";
char ImGuiFunctionality::quotes[128 * 10] = "";