#include "Cell.h"
#include "TextureInit/TextureInit.h"
#include "Game/Character.h"
#include "Game/Enemy.h"
#include "Scene/Game/Game.h"

void Cell::SetItemContainer(std::vector<std::shared_ptr<Item>> items)
{
	itemContainer = std::make_shared<ItemContainer>(items);
}

void Cell::Interact()
{
	try
	{
		if (walkable) interactionOnTop();
		else interactionFromNeighbour();
	}
	catch (const std::exception&)
	{
		//printf("Cell does not have a bound interaction\n");
	}
}
/// <summary>
/// ALWAYS SET EMPTY TO TRUE IF f IS EMPTY
/// </summary>
/// <param name="f"></param>
/// <param name="empty"></param>
void Cell::SetInteraction(std::function<void()> f, bool empty)
{
	if (walkable) interactionOnTop = f;
	else interactionFromNeighbour = f;
	interactable = !empty;
}
Cell::Cell(unsigned int x, unsigned int y, unsigned int texId)
	: position({ x, y }), textureId(texId), walkable(false), interactable(false)
{
	SetInteraction([]() {},true);
	if (texId == TextureInit::textureMapSToI["dungeonGround"] ||
		texId == TextureInit::textureMapSToI["dungeonDownstairs"] || 
		texId == TextureInit::textureMapSToI["dungeonUpstairs"])
		walkable = true;
	
}
void Cell::SetNeighbours(std::shared_ptr<Cell> up, std::shared_ptr<Cell> right, std::shared_ptr<Cell> down, std::shared_ptr<Cell> left)
{
	neighbours[0] = up;
	neighbours[1] = right;
	neighbours[2] = down;
	neighbours[3] = left;
}
void Cell::Search(std::stack<std::shared_ptr<Cell>> searchingStack, int depth, std::shared_ptr<Cell> prev)
{
	// ALWAYS USE currentCell INSTEAD OF this IN THIS FUNCTION ORELSE LOGIC WILL NOT WORK
	if (searchingStack.empty() || depth == 6) return;
	std::shared_ptr<Cell> currentCell = searchingStack.top(); 
	searchingStack.pop();

	if (currentlyVisible.count(currentCell->position) > 0 &&
		depth > currentlyVisible[currentCell->position]) return;
	currentlyVisible[(currentCell->position)] = depth;

	if (std::shared_ptr<Enemy> enemy = currentCell->enemy) {
		enemy->visible = true;
		enemy->shader->SetUniform1f("u_Desaturation", float(depth)/6);
		if (depth < 6) {
			if ((depth < 4 || enemy->character)&& prev && enemy->lockedInAnimation == -1) {
				if (prev->enemy == nullptr)
					enemy->aggroed = prev->position;
			}
		}
	}
	if (std::shared_ptr<TexturedRect> object = currentCell->textureObject) {
		object->visible = true;
		object->shader->SetUniform1f("u_Desaturation", float(depth) / 6);
	}
	if (!currentCell->items.empty()) {
		std::shared_ptr<Item> item = currentCell->items.top();
		item->floorTexture->visible = true;
		item->floorTexture->shader->SetUniform1f("u_Desaturation", float(depth) / 6);
	}
	
	checked[currentCell->position] = depth;
	
	if (!currentCell->walkable && !currentCell->enemy) return;
	
	for (int i = 0; i < 4; i++) {
		if (currentCell->neighbours[i] != nullptr) {
			searchingStack.push(currentCell->neighbours[i]);
		}
		Search(searchingStack, depth+1,currentCell);
	}
}
//this search fucks up the prev cell attribute so doesnt really work
/*void Cell::Search(std::queue<std::pair<std::shared_ptr<Cell>, int>>& searchingQueue, std::shared_ptr<Cell> prev) {
	while (!searchingQueue.empty()) {
		auto [currentCell, depth] = searchingQueue.front();
		searchingQueue.pop();

		if (depth == 6) continue;

		if (currentlyVisible.find(currentCell->position) != currentlyVisible.end() &&
			depth > currentlyVisible[currentCell->position]) continue;
		currentlyVisible[(currentCell->position)] = depth;

		if (enemy = currentCell->enemy) {
			enemy->visible = true;
			enemy->shader->SetUniform1f("u_Desaturation", float(depth) / 6);
			if (depth < 6) {
				if (depth < 4 && prev) {
					enemy->aggroed = prev->position;
				}
			}
		}

		if (checked.find(currentCell->position) != checked.end() && depth > checked[currentCell->position]) continue;
		checked.emplace(std::make_pair(currentCell->position, depth));

		if (!currentCell->walkable && !currentCell->enemy) continue;

		for (int i = 0; i < 4; i++) {
			if (currentCell->neighbours[i] != nullptr) {
				searchingQueue.push(std::make_pair(currentCell->neighbours[i], depth + 1));
			}
		}
	}
}*/
// could prob be made faster with dynamic programming but cba
bool Cell::CheckSolvability(std::pair<unsigned int, unsigned int> end, 
	std::set<std::pair<unsigned int, unsigned int>>& checkedMap, 
	std::stack<std::shared_ptr<Cell>>& searchingStack)
{
	while (!searchingStack.empty()) {
		std::shared_ptr<Cell> currentCell = searchingStack.top();
		searchingStack.pop();
		checkedMap.emplace(currentCell->position);
		if (currentCell->position == end) {
			//printf("Total cells visited in pro algorithm: %i\n",int(checkedMap.size()));
			//std::cout << "Checked Succeed: " << checkedMap.size() << "\n";
			//std::cout << "Searching stack Succeed: " << searchingStack.size() << "\n";
			return true;
		}

		glm::vec2 direction = glm::vec2(((float)end.first - currentCell->position.first), ((float)end.second - currentCell->position.second));

		//std::cout << "Current cell: " << currentCell->position.first << "|" << currentCell->position.second<< "\n" 
		//	<< "End: " << end.first<< " | " << end.second<< "\n";
		//std::cout << "Vector: " << direction.x << "/" << direction.y << "\n"; 
		
		//napishi casove ako direction.x/y == 0
		int first,second,third,forth;
		//1
		if (direction.x > 0 && direction.y >= 0 && direction.x >= direction.y) {
			first = RIGHT;
			second = UP;
			third = DOWN;
			forth = LEFT;
			//right = 0;
			//up = 1;
			//down = 2;
			//left = 3;
			//std::cout << "Direction: Right-Up\n";

		}//2
		else if (direction.x > 0 && direction.y <= 0 && direction.x >= -direction.y) {
			first = RIGHT;
			second = DOWN;
			third = UP;
			forth = LEFT;
			//right = 0;
			//down = 1;
			//up = 2;
			//left = 3;
			//std::cout << "Direction: Right-Down\n";
		}//3
		else if (direction.x >= 0 && direction.y < 0 && direction.x <= -direction.y) {
			first = DOWN;
			second = RIGHT;
			third = LEFT;
			forth = UP;
			//down = 0;
			//right = 1;
			//left = 2;
			//up = 3;
			//std::cout << "Direction: Down-Right\n";
		}//4
		else if (direction.x <= 0 && direction.y < 0 && -direction.x >= -direction.y) {
			first = DOWN;
			second = LEFT;
			third = RIGHT;
			forth = UP;
			//down = 0;
			//left = 1;
			//right = 2;
			//up = 3;
			//std::cout << "Direction: Down-Left\n";
		}//5
		else if (direction.x < 0 && direction.y <= 0 && -direction.x <= -direction.y) {
			first = LEFT;
			second = DOWN;
			third = UP;
			forth = RIGHT;
			//left = 0;
			//down = 1;
			//up = 2;
			//right = 3;
			//std::cout << "Direction: Left-Down\n";
		}//6
		else if (direction.x < 0 && direction.y >= 0 && -direction.x >= direction.y) {
			first = LEFT;
			second = UP;
			third = DOWN;
			forth = RIGHT;
			//left = 0;
			//up = 1;
			//down = 2;
			//right = 3;
			//std::cout << "Direction: Left-Up\n";
		}//7
		else if (direction.x <= 0 && direction.y > 0 && -direction.x <= direction.y) {
			first = UP;
			second = LEFT;
			third = RIGHT;
			forth = DOWN;
			//up = 0;
			//left = 1;
			//right = 2;
			//down = 3;
			//std::cout << "Direction: Up-Left\n";
		}//8
		else if (direction.x >= 0 && direction.y > 0 && -direction.x <= direction.y) {
			first = UP;
			second = RIGHT;
			third = LEFT;
			forth = DOWN;
			//up = 0;
			//right = 1;
			//left = 2;
			//down = 3;
			//std::cout << "Direction: Up-Right\n";
		}
		else {
			//std::cout << "wtf happened\n";
			//std::cout << currentCell->position.first << "|" << currentCell->position.second << "\n";
			//std::cout << direction.x<< "|" << direction.y << "\n";
		}
		if (currentCell->neighbours[forth] != nullptr
			&& checkedMap.find(currentCell->neighbours[forth]->position) == checkedMap.end()
			&& (currentCell->neighbours[forth]->walkable 
			|| (currentCell->neighbours[forth]->textureObject && currentCell->neighbours[forth]->textureObject->texture == "door"))
			)
			searchingStack.push(currentCell->neighbours[forth]);

		if (currentCell->neighbours[third] != nullptr
			&& checkedMap.find(currentCell->neighbours[third]->position) == checkedMap.end()
			&& (currentCell->neighbours[third]->walkable || (currentCell->neighbours[third]->textureObject && currentCell->neighbours[third]->textureObject->texture == "door"))
			)
			searchingStack.push(currentCell->neighbours[third]);

		if (currentCell->neighbours[second] != nullptr
			&& checkedMap.find(currentCell->neighbours[second]->position) == checkedMap.end()
			&& (currentCell->neighbours[second]->walkable || (currentCell->neighbours[second]->textureObject && currentCell->neighbours[second]->textureObject->texture == "door"))
			)
			searchingStack.push(currentCell->neighbours[second]);

		if (currentCell->neighbours[first] != nullptr
			&& checkedMap.find(currentCell->neighbours[first]->position) == checkedMap.end()
			&& (currentCell->neighbours[first]->walkable || (currentCell->neighbours[first]->textureObject && currentCell->neighbours[first]->textureObject->texture == "door"))
			)
			searchingStack.push(currentCell->neighbours[first]);
		//std::stack<std::shared_ptr<Cell>> tmp(searchingStack);
		//while (!tmp.empty()) {
		//	std::cout << tmp.top()->position.second << "/" << tmp.top()->position.first << "\t";
		//	tmp.pop();
		//}
		//std::cout << "\n";
	}
	//std::cout << "Checked Failed: " << checkedMap.size() << "\n";
	//std::cout << "Searching stack Succeed: " << searchingStack.size() << "\n";
	return false;
}
// implement shared sarching stack inbetween enemies
std::shared_ptr<Cell> Cell::FindCharacter(std::shared_ptr<Cell> characterCell, std::shared_ptr<Cell> enemyCell, int maxDepth)
{
	std::stack<std::pair<std::shared_ptr<Cell>, int>> searchingStack;
	searchingStack.emplace(enemyCell, 0);
	std::map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> path;
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> checkedMap;
	

	while (!searchingStack.empty()) {
		auto [currentCell, currentDepth] = searchingStack.top();
		
		
		searchingStack.pop();

		// Store the cell's position and current depth in the map
		checkedMap.emplace(std::make_pair(currentCell->position, currentDepth));

		// If a character is found, return the first step towards it
		if (currentCell->character) {
			//std::cout << "Found character at depth " << currentDepth << "\n";
			std::shared_ptr<Cell> tmp = currentCell;
			std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int>> pathInd;
			for (std::pair<std::shared_ptr<Cell>, std::shared_ptr<Cell>> cell : path) {
				std::pair<unsigned int, unsigned int> firstPos = {-1,-1};
				std::pair<unsigned int, unsigned int> secondPos = {-1,-1};
				if (cell.first) firstPos = cell.first->position;
				if (cell.second) secondPos = cell.second->position;
				pathInd.emplace(std::make_pair(firstPos, secondPos));
			}
			for (int i = 0; path[path[tmp]]; i++) {
				//if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(character->scene)) {
				//	game->indicators.push_back(std::make_shared<FloatingIndicator>(
				//		tmp->position.first * LevelDataManagement::cellSize - Mesh::cameraTranslate.x + LevelDataManagement::cellSize / 2,
				//		tmp->position.second * LevelDataManagement::cellSize - Mesh::cameraTranslate.y + LevelDataManagement::cellSize / 2,
				//		16, 16, std::to_string(i), glm::vec3(1, 1, 1)));
				//}
				tmp = path[tmp];
			}
			return tmp;
		}
		if (currentDepth >= maxDepth) continue;
		glm::vec2 direction = glm::vec2(
			((float)characterCell->position.first - currentCell->position.first),
			((float)characterCell->position.second - currentCell->position.second));

		int first, second, third, forth;

		bool randBit = rand() % 2;

		// Determine neighbor traversal order based on direction
		if (std::abs(direction.x) > std::abs(direction.y) + randBit)
		{
			if (direction.x > 0 + randBit) {
				first = RIGHT;
				forth = LEFT;
			}
			else {
				first = LEFT;
				forth = RIGHT;
			}
			if (direction.y > 0 + randBit) {
				second = UP;
				third = DOWN;
			}
			else{
				second = DOWN;
				third = UP;
			}
		}
		else {
			if (direction.x > 0 + randBit) {
				second = RIGHT;
				third  = LEFT;
			}
			else{
				second = LEFT;
				third  = RIGHT;
			}
			if (direction.y > 0 + randBit) {
				first = UP;
				forth = DOWN;
			}
			else {
				first = DOWN;
				forth = UP;
			}
		}
		

		// Check the neighbors in order
		for (int direction : {forth, third, second, first}) {
			if (currentCell->neighbours[direction] != nullptr
				&& checkedMap.find(currentCell->neighbours[direction]->position) == checkedMap.end()
				&& (currentCell->neighbours[direction]->walkable ||
					(currentCell->neighbours[direction]->textureObject && currentCell->neighbours[direction]->textureObject->texture == "door")))
			{
				// Record the first step from the starting cell
				//if (currentDepth == 0 && firstNextCell == nullptr && direction == first) {
				//	firstNextCell = currentCell->neighbours[direction];
				//}

				path.emplace(currentCell->neighbours[direction], currentCell);

				// Push the neighbor onto the stack
				searchingStack.push({ currentCell->neighbours[direction], currentDepth + 1 });
			}
		}
	}
	// If the character was not found, return nullptr
	std::cout << "Character not found.\n";
	return nullptr;
}
/*bool Cell::CheckSolvability(int depth, std::pair<unsigned int, unsigned int> end, bool)
{
	while (!searchingStack.empty()) {
		std::shared_ptr<Cell> currentCell = searchingStack.top();
		searchingStack.pop();
		checked.emplace(std::make_pair(currentCell->position, depth));
		if (!currentCell->walkable) continue;
		if (currentCell->position == end) {
			printf("Total cells visited in noob algorithm: %i\n", int(checked.size()));
			return true;
		}

		glm::vec2 direction = glm::vec2(((float)end.first - currentCell->position.first), ((float)end.second - currentCell->position.second));

		for (int i = 0; i < 4; i++) {
			if (currentCell->neighbours[i] != nullptr
				&& checked.find(currentCell->neighbours[i]->position) == checked.end()
				&& currentCell->neighbours[i]->walkable
				)
			searchingStack.push(currentCell->neighbours[i]);
		}
	}
	return false;
}*/
std::map<std::pair<unsigned int, unsigned int>, unsigned int> Cell::checked;
std::map<std::pair<unsigned int, unsigned int>, unsigned int> Cell::currentlyVisible;
