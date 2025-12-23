#pragma once
#include "Scene/Scene.h"
#include "OpenGL/Environment.h"
#include "OpenGL/Selector.h"
#include <OpenGL/Text.h>
#include "Scene/Menu/Menu.h"
#include "Game/NPC.h"
#include "Game/Character.h"
#include "Game/Enemy.h"
#include "OpenGL/MapVisualization.h"
#include "Game/FloatingIndicator.h"

#include <map>
#include <Game/Bar.h>
#include <chrono>
#include <queue>

class Menu;
class Game : public Scene, public std::enable_shared_from_this<Game>
{
	long timeUnits;

public:
	std::shared_ptr<Cell> selectedCell;

	bool deleteThis;

	std::pair<float, float> mouseLocation;
	std::shared_ptr<Selector> selectorSelecting;

	std::shared_ptr<TexturedRect> test;
	std::shared_ptr<Text> counter;

	std::shared_ptr<Menu> menu;
	std::shared_ptr<DialogBox> dialogbox;
	std::shared_ptr<Environment> environment;
	std::shared_ptr<MapVisualization> mapv;
	std::shared_ptr<Character> character;
	std::map<std::string, std::shared_ptr<NPC>> npcs;
	std::map<std::string, std::shared_ptr<Enemy>> enemies;
	std::shared_ptr<Text> text;
	std::shared_ptr<Bar> healthbar;
	std::shared_ptr<Bar> xpbar;
	std::shared_ptr<TexturedRect> healthbarBorder;
	std::shared_ptr<TexturedRect> xpbarBorder;
	std::shared_ptr<Text> healthBarText;
	std::shared_ptr<Text> xpbarText;
	std::priority_queue<std::shared_ptr<Enemy>,std::vector<std::shared_ptr<Enemy>>,Enemy::CompareEnemyY> ydistanceEnemies;

	std::vector<std::shared_ptr<FloatingIndicator>> indicators;

	void UpdateFloatingIndicators();
	void DrawFloatingIndicators();
	void SearchCells();
	void InitializeEnemies();
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;

	std::chrono::steady_clock::time_point lastClickTime;
	const std::chrono::milliseconds minTimeBetweenClicks = std::chrono::milliseconds(200);
	bool InputEnabled();

	static long tickUnits;
	static long prevTickUnit;

	bool movingEnemies;
};

