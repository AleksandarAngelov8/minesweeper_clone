#pragma once

#include "Game/Multiplayer/Player.h"

#include "Scene/Scene.h"
#include <OpenGL/Text.h>
#include "Scene/Menu/Menu.h"
#include "Game/FloatingIndicator.h"
#include "OpenGL/TexturedRect.h"

#include <map>
#include <Game/Bar.h>
#include <chrono>
#include <queue>
#include "Game/Multiplayer/Missile.h"
#include <array>

class Menu;
class Arena : public Scene, public std::enable_shared_from_this<Arena>
{
	long timeUnits;
	glm::vec2 mousePos;
public:
	bool triggerCleanupSectors;
	// wtf is 60/16 and 34/16
	std::array<std::array<std::set<std::weak_ptr<Missile>, std::owner_less<std::weak_ptr<Missile>>>, 60 / 16>, 34 / 16> sectors;
	int deathTrigger;
	std::shared_ptr<TexturedRect> circle;
	std::shared_ptr<Player> currentPlayer;

	std::chrono::steady_clock::time_point prevTimeStamp;
	std::chrono::steady_clock::time_point currentTimeStamp;
	std::chrono::milliseconds elapsedTime;

	std::shared_ptr<Player> character, enemy;

	std::vector<std::shared_ptr<FloatingIndicator>> indicators;

	std::shared_ptr<Menu> menu;

	void CleanupSectors();
	void Synchronize(std::string command, std::vector<std::string> messageMap);
	void UpdateFloatingIndicators();
	void DrawFloatingIndicators();

	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;

	const std::chrono::milliseconds minTimeBetweenClicks = std::chrono::milliseconds(200);

	std::chrono::milliseconds animationSwapTime;
};

