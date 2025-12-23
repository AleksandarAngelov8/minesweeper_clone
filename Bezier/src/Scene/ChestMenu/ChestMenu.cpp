#include "ChestMenu.h"
#include "Button/ButtonFunctionality.h"
#include "Game/Equipment.h"
#include "Game/FloatingIndicator.h"
#include "Scene/ItemMenu/ItemMenu.h"
#include "Scene/Game/Game.h"

void ChestMenu::SetUp(std::shared_ptr<Scene> parentScene)
{
	setup = 1;
	this->parentScene = parentScene;

	std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(parentScene);
	gameScene = game;

	itemMenu = std::dynamic_pointer_cast<ItemMenu>(ButtonFunctionality::scenes["itemMenu"]);
	itemMenu->SetUp(game);
	
	//itemContainerInventory = game->character->inventory;
	//SetUpItemsInventory();

	itemContainerChest = game->character->currentCell->neighbours[game->character->rotation]->itemContainer;
	
	SetUpItemsChest();

	//menuBackgroundInventory = std::make_shared<TexturedRect>(2 * Resolution::WIDTH / 7 - 544 / 2, Resolution::HEIGHT / 2 - 544 / 2 - 50, 544, 544, "itemMenu");
	menuBackgroundChest = std::make_shared<TexturedRect>(5 * Resolution::WIDTH / 7 - 544 / 2, Resolution::HEIGHT / 2 - 544 / 2 - 50, 544, 544, "itemMenu");

	selector = std::make_shared<Selector>(5 * Resolution::WIDTH / 7 - 544 / 2 + 32, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - 50,
		LevelDataManagement::cellSize, LevelDataManagement::cellSize, glm::vec3(1.0, 0.0, 1.0));

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			/*
			(2 + 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + j * 128,
				Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - i * 128 - 50 + 96 / 2,
				96, 96,
				*/
			std::shared_ptr<Button> tmpButton = std::make_shared<Button>(
				(i) * 4 + j,
				(2) * Resolution::WIDTH / 7 - 544 / 2 + 32 + j * 128 + 96/2,
				Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - i * 128 - 50 + 96 / 2,
				96, 96,
				glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), "", (i == 0 && j == 0));
			if ((i) * 4 + j < itemMenu->itemContainer->items.size()) {
				tmpButton->SetAction([this]() {
					MoveItem(itemMenu->itemContainer,itemContainerChest);
					itemMenu->SetUpItems();
					SetUpItemsChest();
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
			}
			if (j < 3) {
				buttons[index]->SetNeighbourRight(buttons[index + 1]);
			}
			if (i > 0) {
				buttons[index]->SetNeighbourUp(buttons[index - 4]);
			}
			if (i < 3) {
				buttons[index]->SetNeighbourDown(buttons[index + 4]);
			}
		}
	}
	currentActiveButtonIndex = 1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			//(2 + right * 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + x * 128, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - y * 128 - 50, 96, 96, texture);
			std::shared_ptr<Button> tmpButton = std::make_shared<Button>(16 + (i) * 4 + j,
				(2 + 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + j * 128 + 96/2,
				Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - i * 128 - 50 + 96 / 2,
				96, 96,
				glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), "", (i == 0 && j == 0));
			if ((i) * 4 + j < itemContainerChest->items.size()) {
				tmpButton->SetAction([this]() {
					MoveItem(itemContainerChest, itemMenu->itemContainer);
					itemMenu->SetUpItems();
					SetUpItemsChest();
					});
			}
			buttons.emplace(16 + (i) * 4 + j, tmpButton);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int index = 16 + 4 * i + j;
			if (j > 0) {
				buttons[index]->SetNeighbourLeft(buttons[index - 1]);
			}
			if (j < 3) {
				buttons[index]->SetNeighbourRight(buttons[index + 1]);
			}
			if (i > 0) {
				buttons[index]->SetNeighbourUp(buttons[index - 4]);
			}
			if (i < 3) {
				buttons[index]->SetNeighbourDown(buttons[index + 4]);
			}
		}
	}
	for (int i = 16; i < 32; i+=4) {
		buttons[i]->SetNeighbourLeft(buttons[i-13]);
		buttons[i - 13]->SetNeighbourRight(buttons[i]);
	}
	
	Scene::SetUp();
}
void ChestMenu::MoveItem(std::shared_ptr<ItemContainer> from, std::shared_ptr<ItemContainer> to) {
	int currentActiveTmp = currentActiveButtonIndex;
	currentActiveTmp %= 16;
	bool indexIsRight = currentActiveButtonIndex >= 16;
	std::shared_ptr<Item> item = from->items[currentActiveTmp];
	std::array<std::shared_ptr<ItemContainer>, 2> itemContainers = { itemMenu->itemContainer, itemContainerChest };
	if (item->count == 1)
	{
		buttons[(16 * indexIsRight) + itemContainers[indexIsRight]->items.size() - 1]->SetAction([]() {});
		from->RemoveItem(currentActiveTmp);
	}
	else {
		item->count--;
	}
	if (!to->Contains(item->GetLabel())) {
		if (!item->stackable) {
			if (std::shared_ptr<Equipment> eq = std::dynamic_pointer_cast<Equipment>(item); eq && eq->equipped) {
				from->UnequipWeapon(eq);
				gameScene->indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH/2,Resolution::HEIGHT/2,32,32,"Unequipped", glm::vec3(1.0,0.0,0.0)));
			}
			to->AddItem(item);
		}
		else
			to->AddItem(std::make_shared<Item>(item->GetLabel(), 1, item->texture, item->stackable));
		buttons[(16 * (1 - indexIsRight)) + itemContainers[1 - indexIsRight]->items.size() - 1]->SetAction([this, itemContainers, indexIsRight]() {
			MoveItem(itemContainers[1 - indexIsRight], itemContainers[indexIsRight]);
			itemMenu->SetUpItems();
			SetUpItemsChest();
			});
	}
	else {
		to->GetItem(item->GetLabel())->count++;
	}
}
void ChestMenu::SetUpItemsChest()
{
	int x = 0, y = 0;
	for (const auto& item : itemContainerChest->items) {
		if (x == 4) {
			x = 0;
			y++;
		}
		item->SetTextureInInventory(x, y, 1);
		x++;
	}
}
void ChestMenu::Update(float xpos, float ypos)
{
	std::cout << currentActiveButtonIndex << " - 1\n";
	Scene::Update(xpos, ypos);
	std::cout << currentActiveButtonIndex << " - 2\n";
	selector->translation = glm::vec3(buttons[currentActiveButtonIndex]->minX + 48, buttons[currentActiveButtonIndex]->minY + 48, 0);
	std::cout << currentActiveButtonIndex << " - 3\n";
	gameScene->Update(xpos,ypos);
	std::cout << currentActiveButtonIndex << " - 4\n";
}
void ChestMenu::Draw(bool fromChild)
{
	if (parentScene)
		parentScene->Draw();
	itemMenu->menuBackground->Draw();
	menuBackgroundChest->Draw();
	for (const auto& item : itemMenu->itemContainer->items) {
		if (std::shared_ptr<Equipment> eq = std::dynamic_pointer_cast<Equipment>(item); eq && eq->equipped) 
			eq->equippedSymbol->Draw();

		item->inventoryTexture->Draw();
		if (item->count > 1)
			item->countText->Draw();
	}
	for (const auto& item : itemContainerChest->items) {
		item->inventoryTexture->Draw();
		if (item->count > 1)
			item->countText->Draw();
	}

	selector->Draw();

	gameScene->DrawFloatingIndicators();
	//std::cout << "Pointer from ChestMenu side: " << parentScene.get() << "\n";
	//std::cout << "Pointer from ChestMenu side: " << shared_from_this() << "\n";
	//std::cout << currentActiveButtonIndex << "CURRENTBUTTON\n";
	Scene::Draw();
}
void ChestMenu::PerformAction(int pressedButtonId)
{
	switch (pressedButtonId)
	{
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		buttons[currentActiveButtonIndex]->Activate();
		break;
	case GLFW_KEY_UP:
		if (buttons[currentActiveButtonIndex]->neighbours[0] != NULL) 
		{
			//selector->translation += glm::vec3(0.0, 128.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[0]->id;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (buttons[currentActiveButtonIndex]->neighbours[1] != NULL) 
		{
			if (currentActiveButtonIndex == 3 || currentActiveButtonIndex == 7 || currentActiveButtonIndex == 11 || currentActiveButtonIndex == 15)
				selector->translation += glm::vec3(310, 0, 0.0);
			//selector->translation += glm::vec3(128.0, 0.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[1]->id;
		}
		break;
	case GLFW_KEY_DOWN:
		if (buttons[currentActiveButtonIndex]->neighbours[2] != NULL) 
		{
			//selector->translation += glm::vec3(0.0, -128.0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[2]->id;
		}
		break;
	case GLFW_KEY_LEFT:
		if (buttons[currentActiveButtonIndex]->neighbours[3] != NULL) 
		{
			if (currentActiveButtonIndex == 16 || currentActiveButtonIndex == 20 || currentActiveButtonIndex == 24 || currentActiveButtonIndex == 28)
				selector->translation += glm::vec3(-310, 0, 0.0);
			//selector->translation += glm::vec3(-128.0, 0, 0.0);
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[3]->id;
		}
		break;
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		ButtonFunctionality::ChangeScene("game", false);
		break;
	default:
		break;
	}
}
void ChestMenu::PerformActionScroll(double ypos)
{
}

void ChestMenu::ReceiveInfo(char* data)
{
}

