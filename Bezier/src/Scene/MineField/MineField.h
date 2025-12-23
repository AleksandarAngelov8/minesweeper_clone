#pragma once

#include "Game/Multiplayer/Player.h"

#include "Scene/Scene.h"
#include <OpenGL/Text.h>
#include "Scene/Menu/Menu.h"
#include "Game/FloatingIndicator.h"
#include "OpenGL/TexturedRect.h"

#include <map>
#include <chrono>
#include <queue>
#include <array>

class Menu;
class MineField : public Scene, public std::enable_shared_from_this<MineField>
{
	long timeUnits;
	glm::vec2 mousePos;
public:
	std::unique_ptr<TexturedRect> rect;

	std::vector<std::shared_ptr<FloatingIndicator>> indicators;

	std::shared_ptr<Menu> menu;

	void UpdateFloatingIndicators();
	void DrawFloatingIndicators();

	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

