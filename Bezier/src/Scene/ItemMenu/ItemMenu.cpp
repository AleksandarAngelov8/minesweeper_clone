#include "ItemMenu.h"
#include "Button/ButtonFunctionality.h"
#include "Game/Equipment.h"
#include "Scene/Game/Game.h"
void ItemMenu::SetUp(std::shared_ptr<Scene> parentScene)
{
	setup = 1;
	this->parentScene = parentScene;
	if (std::shared_ptr<Menu> itemMenu = std::dynamic_pointer_cast<Menu>(parentScene)) {
		gameScene = std::dynamic_pointer_cast<Game>(itemMenu->parentScene);
	}
	else if (gameScene = std::dynamic_pointer_cast<Game>(parentScene)) {}

	itemContainer = gameScene->character->inventory;

	SetUpItems();

	menuBackground = std::make_shared<TexturedRect>(2*Resolution::WIDTH / 7 - 544 / 2, Resolution::HEIGHT / 2 - 544 / 2 - 50, 544, 544, "itemMenu");
	selector = std::make_shared<Selector>(2 * Resolution::WIDTH / 7 - 544 / 2 + 32 , Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - 50,
		LevelDataManagement::cellSize, LevelDataManagement::cellSize,glm::vec3(1.0,0.0,1.0));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::shared_ptr<Button> tmpButton = std::make_shared<Button>(
				i * 4 + j, (2) * Resolution::WIDTH / 7 - 544 / 2 + 32 + j * 128 + 96/2,
				Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - i * 128 - 50 + 96/2,
				96, 96, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), "", (i == 0 && j == 0));
			if ((i) * 4 + j < itemContainer->items.size()) {
				tmpButton->SetAction([]() {
					ButtonFunctionality::ChangeScene("itemSelect");
					});
			}
			buttons.emplace((i) * 4 + j, tmpButton);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int index = 4 * i + j;
			if (j > 0) {
				buttons[index]->SetNeighbourLeft(buttons[index - 1]);
				//std::cout << i << "/" << j << "-LEFT\n";
			}
			if (j < 3) {
				buttons[index]->SetNeighbourRight(buttons[index + 1]);
				//std::cout << i << "/" << j << "-RIGHT\n";
			}
			if (i > 0) {
				buttons[index]->SetNeighbourUp(buttons[index - 4]);
				//std::cout << i << "/" << j << "-DOWN\n";
			}
			if (i < 3) {
				buttons[index]->SetNeighbourDown(buttons[index + 4]);
				//std::cout << i << "/" << j << "-UP\n";
			}
		}
	}

	currentActiveButtonIndex = 0;

	Scene::SetUp();
}
void ItemMenu::SetUpItems()
{
	int x = 0, y = 0;
	for (int i = 0; i < itemContainer->items.size(); i++)
	{
		std::shared_ptr<Item> item = itemContainer->items[i];
		if (item->count < 1) {
			itemContainer->items.erase(itemContainer->items.begin() + i);
			i--;
			continue;
		}
		item->SetTextureInInventory(x, y);
		x++;
		if (x == 4) {
			x = 0;
			y++;
		}
	}
}
void ItemMenu::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	selector->translation = glm::vec3(buttons[currentActiveButtonIndex]->minX + 48, buttons[currentActiveButtonIndex]->minY+48,0);
	gameScene->Update(xpos,ypos);
}
void ItemMenu::Draw(bool fromChild)
{
	if (parentScene)
		parentScene->Draw(1);
	menuBackground->Draw();
	for (const auto& item : itemContainer->items) {
		if (std::shared_ptr<Equipment> eq = std::dynamic_pointer_cast<Equipment>(item); eq && eq->equipped)
			eq->equippedSymbol->Draw();
		item->inventoryTexture->Draw();
		if (item->count > 1)
			item->countText->Draw();

	}
	selector->Draw();
	gameScene->DrawFloatingIndicators();
	//std::cout << "Pointer from ItemMenu side: " << parentScene.get() << "\n";
	//std::cout << "Pointer from ItemMenu side: " << shared_from_this() << "\n";
	//std::cout << currentActiveButtonIndex << "CURRENTBUTTON\n";
	if (!fromChild) Scene::Draw();
}
void ItemMenu::PerformAction(int pressedButtonId)
{
	switch (pressedButtonId)
	{
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		buttons[currentActiveButtonIndex]->Activate();
		break;
	case GLFW_KEY_UP:
		if (buttons[currentActiveButtonIndex]->neighbours[0] != NULL) {
			//selector->translation += glm::vec3(0.0, 128.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[0]->id;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (buttons[currentActiveButtonIndex]->neighbours[1] != NULL) {
			//selector->translation += glm::vec3(128.0, 0.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[1]->id;
		}
		break;
	case GLFW_KEY_DOWN:
		if (buttons[currentActiveButtonIndex]->neighbours[2] != NULL) {
			//selector->translation += glm::vec3(0.0, -128.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[2]->id;
		}
		break;
	case GLFW_KEY_LEFT:
		if (buttons[currentActiveButtonIndex]->neighbours[3] != NULL) {
			//selector->translation += glm::vec3(-128.0, 0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[3]->id;
		}
		break;
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		ButtonFunctionality::ChangeScene("menu", false);
		break;
	default:
		break;
	}
}
void ItemMenu::PerformActionScroll(double ypos)
{
}

void ItemMenu::ReceiveInfo(char* data)
{
}

