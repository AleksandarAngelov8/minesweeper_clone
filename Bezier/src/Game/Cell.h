#pragma once
#include <utility>
#include <functional>
#include <memory>
#include <stack>
#include <array>
#include <queue>
#include <map>
#include <set>
#include <string>
#include "Game/AttackWarning.h"

class NPC;
class Enemy;
class Character;
class TexturedRect;
class Item;
class Environment;
class ItemContainer;
class Cell : public std::enable_shared_from_this<Cell>
{
private:
	std::function<void()> interactionFromNeighbour;
	std::function<void()> interactionOnTop;
public:
	std::map<int, std::map<std::string,std::shared_ptr<AttackWarning>>> attackWarnings;
	std::shared_ptr<ItemContainer> itemContainer;
	std::shared_ptr<TexturedRect> textureObject;
	std::array<std::shared_ptr<Cell>, 4> neighbours;
	std::shared_ptr<NPC> npc;
	std::shared_ptr<Enemy> enemy;
	std::shared_ptr<Character> character;
	std::stack<std::shared_ptr<Item>> items;
	void SetItemContainer(std::vector<std::shared_ptr<Item>> items);
	void Interact();
	void SetInteraction(std::function<void()> f, bool empty = 0);
	bool walkable;
	bool interactable;
	Cell(unsigned int x, unsigned int y, unsigned int texId);
	void SetNeighbours(std::shared_ptr<Cell> up, std::shared_ptr<Cell> right, std::shared_ptr<Cell> down, std::shared_ptr<Cell> left);
	std::pair<unsigned int, unsigned int> position;
	unsigned int textureId;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> checked;
	static std::map<std::pair<unsigned int, unsigned int>, unsigned int> currentlyVisible;

	void Search(std::stack<std::shared_ptr<Cell>> searchingStack, int depth, std::shared_ptr<Cell> prev);
	bool CheckSolvability(std::pair<unsigned int, unsigned int> end, std::set<std::pair<unsigned int, unsigned int>>& checkedMap, std::stack<std::shared_ptr<Cell>>& searchingStack);
	std::shared_ptr<Cell> FindCharacter(std::shared_ptr<Cell> characterCell, std::shared_ptr<Cell> enemyCell, int maxDepth = 20);
	//void Search(std::queue<std::pair<std::shared_ptr<Cell>, int>>& searchingQueue, std::shared_ptr<Cell> prev);
	//bool CheckSolvability(int depth, std::pair<unsigned int, unsigned int> end, bool lol);
};
