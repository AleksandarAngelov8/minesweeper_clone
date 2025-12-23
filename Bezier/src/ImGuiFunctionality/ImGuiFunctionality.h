#include "Button/ButtonFunctionality.h"
#include "TextureInit/TextureInit.h"
#include <LevelDataManagement/LevelDataManagement.h>

#pragma once
class ImGuiFunctionality
{
public:
	static char textureName[128];
	static char quotes[128 * 10];
	static float maxHP;
	static float AD;
	static unsigned int level;
	static char name[128];
	static std::string cellData;
	static void ReadCellData(std::shared_ptr<Edit> editScene);
	static void ImGuiKeyButtonCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ImGuiMouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
	static bool ImGuiWantsMouse();
	static void Init();
	static void Update(float xpos, float ypos);
	static void Draw();
	static void ShutDown();
};

