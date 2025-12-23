#include "Game.h"
#include <iostream>
#include <utility>
#include <future>
#include <stack>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
/*
NESHTA KOITO TRQBVA DA SE NAPRAVQT:
stats, enemy, combat
SAVE/LOAD GAME
MENU,SPEECH INTERACTION,COMBAT INITIALIZATION, ENEMY/FRIENDLY NPC
ELYSIA MODEL, TEST FRIENDLY/ENEMY NPC
*/

#define cellSize LevelDataManagement::cellSize
//found bug where when you attack when you are on the unlocked door it breaks
void Game::SetUp(std::shared_ptr<Scene> parentScene)
{// make it so you can damage enemy
	bool solvable;
	long long seedOffSet = 0;
	do
	{
		std::set<std::pair<unsigned int, unsigned int>> checkedMap;
		std::stack<std::shared_ptr<Cell>> searchingStack;
		mapv = std::make_shared<MapVisualization>(100, seedOffSet);
		seedOffSet++;
		environment = std::make_shared<Environment>(shared_from_this(), mapv);
		//printf("KILL YOURSELF %i,%i\n", environment->start.first, environment->start.second);
		character = std::make_shared<Character>(
			LevelDataManagement::cellSize * environment->start.first, LevelDataManagement::cellSize * environment->start.second,
			(int)(LevelDataManagement::cellSize), (int)(LevelDataManagement::cellSize) * 4 / 3, "elysia3", glm::vec2(4, 4), shared_from_this(), 20, 20, 10, 1, 0);
		character->currentCell->character = character;
		searchingStack.push(character->currentCell);
		solvable = character->currentCell->CheckSolvability(environment->end, checkedMap, searchingStack);

	} while (!solvable);

	std::set<std::pair<uint32_t, uint32_t>> set;
	std::transform(Cell::checked.begin(), Cell::checked.end(), std::inserter(set, set.end()), [](const auto& key) {return key.first; });
	LevelDataManagement::ExportEnvironmentText(shared_from_this(), "testing1", set);
	Cell::checked.clear();

	//	Cell::checked[environment->start] = 1;

	setup = 1;
	//Character chara(0, 32, (int)(LevelDataManagement::cellSize), (int)(LevelDataManagement::cellSize) * 3 / 2, 14.0, 4);
	dialogbox = std::make_shared<DialogBox>();
	dialogbox->SetUp();

	//environment = std::make_shared<Environment>(shared_from_this());
	//std::cout << "\n\n";
	for (const auto& enemy : enemies) {
		enemy.second->SetUpCellConnections();
		//std::cout << enemy.second->currentCell->position.first << "/" << enemy.second->currentCell->position.second << "\n";
	}
	for (const auto& npc : npcs) {
		npc.second->SetUpCellConnections();
	}
	//std::vector<std::string> interactions = {"Hello there!"};
	//std::vector<unsigned int> repeatingInteractions = {0};
	//npcs.emplace("npc1", std::make_shared<NPC>(96 * 3, 96 * 5,
	//	(int)(LevelDataManagement::cellSize), (int)(LevelDataManagement::cellSize),
	//	"friendly", 4, shared_from_this(), interactions, repeatingInteractions));

	//text = std::make_shared<Text>(96, 96, 32, 32, "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 !?.,:;-()", glm::vec3(0.3,0.2,0.7), true);


	timeUnits = 0;
	menu = nullptr;

	healthbar = std::make_shared<Bar>(Resolution::WIDTH / 32, 9 * Resolution::HEIGHT / 10, 600.0, 80.0, &character->stats->maxHealth, &character->stats->currentHealth, glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
	healthbarBorder = std::make_shared<TexturedRect>(Resolution::WIDTH / 32 - 10, 9 * Resolution::HEIGHT / 10 - 10, 620.0, 100.0, "healthbarBorder");
	healthBarText = std::make_shared<Text>(Resolution::WIDTH / 32 + 500, 9 * Resolution::HEIGHT / 10 + 40, 32, 32, "HELLO", glm::vec3(1.0, 1.0, 1.0), Text::centered);

	xpbar = std::make_shared<Bar>(Resolution::WIDTH / 32, 8 * Resolution::HEIGHT / 10, 600.0, 15, &character->stats->expToLevelUp, &character->stats->experience, glm::vec3(1.0, 0.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
	xpbarBorder = std::make_shared<TexturedRect>(Resolution::WIDTH / 32 - 5, 8 * Resolution::HEIGHT / 10 - 5, 620.0, 25, "xpBorder");
	xpbarText = std::make_shared<Text>(Resolution::WIDTH / 32 + 500, 8 * Resolution::HEIGHT / 10 + 40, 32, 32, "HELLO", glm::vec3(1.0, 1.0, 1.0), Text::centered);

	lastClickTime = std::chrono::steady_clock::now();
	tickUnits = 0;
	prevTickUnit = -1;

	// testing item on floor:
	{
		auto& cellUnder = character->currentCell->neighbours[UP];
		std::shared_ptr<Item> item = std::make_shared<Item>("deez", 1, "key", 0);
		item->BindItemToCell(environment,shared_from_this(),cellUnder);
		
	}
	// testing chest:
	{
		std::shared_ptr<Cell> cellUnderCell = character->currentCell->neighbours[UP]->neighbours[UP];
		std::vector<std::shared_ptr<Item>> items;
		//items.push_back(std::make_shared<Item>("test1", 1, "slime"));
		//items.push_back(std::make_shared<Item>("test2", 2, "chest"));
		//items.push_back(std::make_shared<Item>("test3", 3, "milk"));
		cellUnderCell->SetItemContainer(items);
		environment->BindInteractableToCell(cellUnderCell->position,"chest", [this, cellUnderCell]() {
			ButtonFunctionality::ChangeScene("chestMenu");
			});
	}
	test = std::make_shared<TexturedRect>(0, 0, 500, 500, "text");
	counter = std::make_shared<Text>(700,500,32,32,"",glm::vec3(1.0,1.0,1.0),Text::right);

	selectorSelecting = std::make_shared<Selector>(0, 0, (int)(cellSize), (int)(cellSize), glm::vec3(1.0, 0.0, 1.0));

	Scene::SetUp();
}
bool Game::InputEnabled()
{
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastClickTime);
	return elapsedTime >= minTimeBetweenClicks;
}
void Game::UpdateFloatingIndicators()
{
	for (int i = 0; i < indicators.size(); i++) {
		if (indicators[i]->shouldBeDeleted) {
			indicators.erase(indicators.begin() + i);
			continue;
		}
		indicators[i]->Update();
	}
}
void Game::DrawFloatingIndicators()
{
	for (const auto& indicator : indicators) {
		indicator->Draw();
	}
}
void Game::SearchCells()
{
	//std::queue<std::pair<std::shared_ptr<Cell>, int>> searchingQueue;
	//searchingQueue.push(std::make_pair(character->currentCell, 0)); 
	//character->currentCell->Search(searchingQueue, nullptr);
	std::stack<std::shared_ptr<Cell>> stack;
	stack.push(character->currentCell);

	//Cell::checked.emplace(character->currentCell->position, );

	character->currentCell->Search(stack, 0,nullptr);
	environment->ChangeCurrentVbo(Cell::checked, Cell::currentlyVisible);
	Cell::currentlyVisible.clear();
}
void Game::InitializeEnemies()
{
}
void Game::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	//Mesh::cameraTranslate = glm::vec3(-Resolution::WIDTH/2,-Resolution::HEIGHT/2,0.0) + character->translation;
	if (menu != nullptr) {
		menu->Update(xpos, ypos);
		return;
	}
	if (dialogbox->shouldBeDrawn) {
		dialogbox->Update(xpos, ypos);
		return;
	}

	
	movingEnemies = false;
	for (auto enemy = enemies.begin(); enemy != enemies.end();) {
		enemy->second->Update();
		if (!enemy->second->alive)
		{
			enemy = enemies.erase(enemy);
		}
		else {
			enemy++;
		}
	}
	if (prevTickUnit < tickUnits) {
		prevTickUnit++;
		environment->Update();

		for (const auto& enemy : enemies)
		{
			//enemy.second->StartMove();
			enemy.second->visible = false;
			if (enemy.second->lockedInAnimation == -1)
				enemy.second->aggroed = { -1,-1 };
		}
		SearchCells();

	}
	
	timeUnits++;
	character->Update();
	if ((character->movingUp || character->movingRight || character->movingDown || character->movingLeft) && timeUnits % 20 == 0) {
		character->IterateAnimationSprite();
	}
	
	healthbar->Update();
	std::string infoS = std::to_string((int)character->stats->currentHealth) + "-" + std::to_string((int)character->stats->maxHealth);
	healthBarText->ChangeText(infoS.c_str());

	xpbar->Update();
	infoS = std::to_string((int)character->stats->experience) + "-" + std::to_string((int)character->stats->expToLevelUp) + ":" + std::to_string(character->stats->level);
	xpbarText->ChangeText(infoS.c_str());

	//REMOVE COMMENT FROM HERE
	UpdateFloatingIndicators();

	for (const auto& enemy : enemies) {
		if (enemy.second->visible)
			ydistanceEnemies.push(enemy.second);
	}
	//test->shader->SetUniform1i("u_TexID", textureIndex);
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastClickTime);
	//counter->ChangeText(std::to_string(elapsedTime.count()).c_str());
	//counter->ChangeText(std::to_string(tickUnits).c_str());
	//counter = Text::ChangeText(counter, std::to_string(tickUnits).c_str());

	mouseLocation = std::make_pair(xpos, ypos);
	unsigned int x = static_cast<unsigned int>((mouseLocation.first+cellSize/2) / cellSize);
	unsigned int y = static_cast<unsigned int>((mouseLocation.second - 12) / cellSize);
	selectorSelecting->translation = glm::vec3(x*cellSize, y*cellSize + cellSize/2 + 12, 0);

	//int x1, y1;
	//x1 = (selectorSelecting->translation.x + Mesh::cameraTranslate.x) / 96;
	//y1 = (selectorSelecting->translation.y + Mesh::cameraTranslate.y) / 96;
	//if (environment->cells[y1][x1] && environment->cells[y1][x1-1] && environment->cells[y1][x1+1]) {
	//	std::shared_ptr<Cell> cellUnderCell = environment->cells[y1][x1];
	//	std::shared_ptr<Cell> cellUnderCellLeft = environment->cells[y1][x1-1];
	//	std::shared_ptr<Cell> cellUnderCellRight = environment->cells[y1][x1+1];
	//	environment->BindInteractableToCell(cellUnderCell->position, "warning_signal_1", []() {});
	//	environment->BindInteractableToCell(cellUnderCellLeft->position, "warning_signal_2", []() {});
	//	environment->BindInteractableToCell(cellUnderCellRight->position, "warning_signal_3", []() {});
	//}
		
	int xCell, yCell;
	xCell = (selectorSelecting->translation.x + Mesh::cameraTranslate.x) / 96;
	yCell = (selectorSelecting->translation.y + Mesh::cameraTranslate.y) / 96;
	selectedCell = environment->cells[yCell][xCell];
}
void Game::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();
	environment->Draw();
	//text->Draw();
	for (const auto& npc : npcs) {
		npc.second->Draw();
	} 
	{
		bool drawnChar = false;
		while (!ydistanceEnemies.empty()) {
			std::shared_ptr<Enemy> currentEnemy = ydistanceEnemies.top();
			ydistanceEnemies.pop();
			currentEnemy->Draw();
			if (!drawnChar && currentEnemy->translation.y <= character->translation.y) {
				character->Draw();
				drawnChar = true;
			}
		}
		if (!drawnChar)character->Draw();
	}
	
	if (menu != nullptr)menu->Draw();
	dialogbox->Draw();
	healthbar->Draw();
	healthbarBorder->Draw();
	healthBarText->Draw();
	
	xpbar->Draw();
	xpbarBorder->Draw();
	xpbarText->Draw();

	DrawFloatingIndicators();
	//test->Draw();
	counter->Draw();
	if (!character->movingDown && 
		!character->movingUp && 
		!character->movingRight && 
		!character->movingLeft) selectorSelecting->Draw();
	Scene::Draw();
}
void Game::PerformAction(int pressedButtonId)
{
	if (menu != nullptr) {
		menu->PerformAction(pressedButtonId);
		if (pressedButtonId == GLFW_KEY_X) {
			menu = nullptr;
			ButtonFunctionality::shouldPerformButtonCheck = 1;
		}
		return;
	}
	if (dialogbox->shouldBeDrawn) {
		dialogbox->PerformAction(pressedButtonId);
		if (pressedButtonId == GLFW_KEY_Z) {
			dialogbox->shouldBeDrawn = 0;
			ButtonFunctionality::shouldPerformButtonCheck = 1;
		}
		if (pressedButtonId == GLFW_KEY_X) {
			dialogbox->shouldBeDrawn = 0;
			ButtonFunctionality::shouldPerformButtonCheck = 1;
		}
		return;
	}
	switch (pressedButtonId)
	{
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		character->InterruptAttack();
		character->StartMoveAnimation(LEFT);
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		character->InterruptAttack();
		character->StartMoveAnimation(RIGHT);
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		character->InterruptAttack();
		character->StartMoveAnimation(DOWN);
		break;
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		character->InterruptAttack();
		character->StartMoveAnimation(UP);
		break;
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		if (character->lockedInAnimation == -1) character->lockedInAnimation = character->turnsToAttack;
		character->ChargeAttack();
		character->Interact();
		Game::tickUnits++;
		//Audio::PlaySound("interact");
		break;
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		if (menu == nullptr) {
			ButtonFunctionality::ChangeScene("menu");
			ButtonFunctionality::shouldPerformButtonCheck = 0;
		}
		else menu = nullptr;
		//Audio::PlaySound("menu");
		break;
	case GLFW_KEY_C:
		character->Stay();
		break;
	case GLFW_KEY_V:
		character->stats->AddExperience(200);
		deleteThis = !deleteThis;
		break;
	case GLFW_KEY_B:
		indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2, Resolution::HEIGHT/ 2,64,64,"+",glm::vec3(1.0,1.0,1.0)));
		break;
	default:
		break;
	}
}
void Game::PerformActionScroll(double ypos)
{
}
void Game::ReceiveInfo(char* data)
{
}
long Game::tickUnits = 0;
long Game::prevTickUnit = -1;