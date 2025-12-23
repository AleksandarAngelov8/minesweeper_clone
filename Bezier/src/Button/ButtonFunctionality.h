#pragma once	

#define BUTTONPLAY 0
#define BUTTONEDIT 2
#define BUTTONEXIT 4

#include <memory>
#include <stdio.h>
#include <map>
#include <string>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Button/ButtonLookup.h"

class Scene;
class NewMenu;
class Game;
class Edit;
class Arena;
class ButtonFunctionality
{
public:
	static bool shouldSetup;
	static float cellSize;
	static char user;

	static std::map<std::string, std::shared_ptr<Scene>> scenes;
	static void ChangeScene(std::string newScene, bool setup = true);
	static void ChangeSceneFromFile(std::shared_ptr<Scene>& newScene, std::string newSceneS);
	static bool shouldPerformButtonCheck;
	static std::string currentSceneId;
	static GLFWwindow* globalWindow;

	static void SetUpScenes();

	static std::string GetCurrentSceneId();
	static std::shared_ptr<Scene> getCurrentSceneObject();
	
	static void PerformButtonCheck();
	static bool CheckPressedKeyboardButton(int key);
	static bool CheckPressedMouseButton(int key);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void ActivateCursor();
	static void DeactivateCursor();


	static void SetWindow(GLFWwindow* window);
	static void CloseWindow();
};

