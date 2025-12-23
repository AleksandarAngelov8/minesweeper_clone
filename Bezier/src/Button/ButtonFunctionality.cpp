#include "ButtonFunctionality.h"
#include <ImGuiFunctionality/ImGuiFunctionality.h>
#include <Scene/NewMenu/NewMenu.h>
#include <Scene/MultiplayerScenes/Arena.h>
#include <Scene/MineField/MineField.h>

void ButtonFunctionality::ChangeScene(std::string newScene, bool setup)
{
	shouldPerformButtonCheck = newScene == "game" || newScene == "arena";
	currentSceneId = newScene;
	std::shared_ptr<Scene> parentScene = nullptr;
	if (setup) {
		if (newScene == "mainMenu") {
			scenes[currentSceneId] = std::make_shared<NewMenu>();
		}
		

		shouldSetup = true;
	}
}
void ButtonFunctionality::CloseWindow()
{
	glfwSetWindowShouldClose(globalWindow,1);
}
void ButtonFunctionality::SetWindow(GLFWwindow* window) {
	globalWindow = window;
}
std::shared_ptr<Scene> ButtonFunctionality::getCurrentSceneObject()
{
	return scenes[currentSceneId];
}
void ButtonFunctionality::SetUpScenes()
{
	ButtonFunctionality::user = user;

	scenes.insert({ "mainMenu",std::make_shared<NewMenu>() });
	scenes.insert({ "minefield", std::make_shared<MineField>() });

	currentSceneId = "mainMenu";
	shouldSetup = true;

	//LevelDataManagement::cellSize = static_cast<float>(LevelDataManagement::zoomCellSizes[4]);
}
std::string ButtonFunctionality::GetCurrentSceneId() {
	return currentSceneId;
}
void ButtonFunctionality::PerformButtonCheck() {
	if (glfwGetKey(globalWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		SetUpScenes();
		//CloseWindow();
	}
	//BUTTONS THAT ARE USED IN ALL OF THE GAME PROCESS
	std::vector<int> buttons{ GLFW_KEY_LEFT ,GLFW_KEY_RIGHT,GLFW_KEY_UP ,GLFW_KEY_DOWN, GLFW_KEY_W,GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SHIFT };
	for (const int& key : buttons) {
		if (glfwGetKey(globalWindow, key) == GLFW_PRESS) {
			getCurrentSceneObject()->PerformAction(key); 
		}
	}
}
bool ButtonFunctionality::CheckPressedKeyboardButton(int key) {
	return (glfwGetKey(globalWindow, key) == GLFW_PRESS);
}
bool ButtonFunctionality::CheckPressedMouseButton(int key) {
	return (glfwGetMouseButton(globalWindow, key) == GLFW_PRESS);
}
void ButtonFunctionality::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (shouldPerformButtonCheck) return;
	ImGuiFunctionality::ImGuiKeyButtonCallBack(window, key, scancode, action, mods);
	if (glfwGetKey(globalWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		SetUpScenes();
		//CloseWindow();
	}
	else if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		getCurrentSceneObject()->PerformAction(key);
	}
}
void ButtonFunctionality::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGuiFunctionality::ImGuiMouseButtonCallBack(window,button,action,mods);
	if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS)
	{
		getCurrentSceneObject()->PerformAction(button);
	}
}
void ButtonFunctionality::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	getCurrentSceneObject()->PerformActionScroll(yoffset);
}
void ButtonFunctionality::ActivateCursor()
{
	glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void ButtonFunctionality::DeactivateCursor()
{
	glfwSetInputMode(globalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow* ButtonFunctionality::globalWindow = NULL;
std::string ButtonFunctionality::currentSceneId = "mainMenu";
std::map<std::string, std::shared_ptr<Scene>> ButtonFunctionality::scenes;
bool ButtonFunctionality::shouldPerformButtonCheck = 0;
char ButtonFunctionality::user = 'x';
float ButtonFunctionality::cellSize = 96.0f;
bool ButtonFunctionality::shouldSetup = false;