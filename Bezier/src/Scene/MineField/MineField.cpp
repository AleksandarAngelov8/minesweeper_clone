#include "MineField.h"

#include <iostream>
#include <utility>
#include <future>
#include <stack>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#define user ButtonFunctionality::user
#define cellSize LevelDataManagement::cellSize
void MineField::SetUp(std::shared_ptr<Scene> parentScene)
{
	rect = std::make_unique<TexturedRect>(0,0,100,100,"slime");
	Scene::SetUp();
}
void MineField::UpdateFloatingIndicators()
{
	for (int i = 0; i < indicators.size(); i++) {
		if (indicators[i]->shouldBeDeleted) {
			indicators.erase(indicators.begin() + i);
			continue;
		}
		indicators[i]->Update();
	}
}
void MineField::DrawFloatingIndicators()
{
	for (const auto& indicator : indicators) {
		indicator->Draw();
	}
}
void MineField::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	if (menu != nullptr) {
		menu->Update(xpos, ypos);
		return;
	}
	mousePos = glm::vec2(xpos, ypos);

	rect->Update();

	timeUnits++;
}
void MineField::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();

	if (menu != nullptr)menu->Draw();

	rect->Draw();

	DrawFloatingIndicators();
	Scene::Draw();
}
void MineField::PerformAction(int pressedButtonId)
{
	if (menu != nullptr) {
		menu->PerformAction(pressedButtonId);
		if (pressedButtonId == GLFW_KEY_X) {
			menu = nullptr;
			ButtonFunctionality::shouldPerformButtonCheck = 1;
		}
		return;
	}
	switch (pressedButtonId)
	{
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		//Audio::PlaySound("interact");
		break;
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		break;
	case GLFW_KEY_C:
		
		break;
	case GLFW_KEY_V:
		break;
	case GLFW_KEY_B:
		indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2, Resolution::HEIGHT / 2, 64, 64, "+", glm::vec3(1.0, 1.0, 1.0)));
		break;
	case GLFW_KEY_LEFT_CONTROL:
		ButtonFunctionality::ActivateCursor();
		break;
	case GLFW_KEY_LEFT_SHIFT:
		ButtonFunctionality::DeactivateCursor();
		break;
	default:
		break;
	}
}
void MineField::PerformActionScroll(double ypos)
{
}
void MineField::ReceiveInfo(char* data)
{
}
