#include "ItemSelect.h"
#include <set>
#include "OpenGL/Resolution.h"
#include "Scene/ItemMenu/ItemMenu.h"
#include "Game/Consumable.h"
#include "Game/Equipment.h"
#include "Scene/Game/Game.h"

void ItemSelect::SetUp(std::shared_ptr<Scene> parentScene)
{
	setup = 1;
	this->parentScene = parentScene;
	float calc300 = static_cast<float>(300.0 / 1920.0 * Resolution::WIDTH);
	float calc200 = static_cast<float>(200.0 / 1920.0 * Resolution::WIDTH);
	float calc100 = static_cast<float>(100.0 / 990.0 * Resolution::HEIGHT);

	std::shared_ptr<ItemMenu> itemMenu = std::dynamic_pointer_cast<ItemMenu>(parentScene);
	std::shared_ptr<Menu> menu = std::dynamic_pointer_cast<Menu>(itemMenu->parentScene);
	std::shared_ptr<Item> item = itemMenu->itemContainer->items[itemMenu->currentActiveButtonIndex];
	gameScene = std::dynamic_pointer_cast<Game>(menu->parentScene);

	std::string action = "X";
	std::shared_ptr<Equipment> eq = std::dynamic_pointer_cast<Equipment>(item);
	std::shared_ptr<Consumable> co = std::dynamic_pointer_cast<Consumable>(item);
	if (eq) {
		if (eq->equipped)
			action = "Unequip";
		else
			action = "Equip";
	}
	else if (co) {
		action = "Use";
	}

	std::shared_ptr<Button> button1 = std::make_shared<Button>(1, Resolution::WIDTH / 2 + (-1 * 1.44 - 0.5) * calc200, calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), action, true);
	std::shared_ptr<Button> button2 = std::make_shared<Button>(2, Resolution::WIDTH / 2 + (0 * 1.44 - 0.5) * calc200, calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "XXXXXXX", false);
	std::shared_ptr<Button> button3 = std::make_shared<Button>(3, Resolution::WIDTH / 2 + (1 * 1.44 - 0.5) * calc200, calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Drop", false);
	std::shared_ptr<Button> button4 = std::make_shared<Button>(4, Resolution::WIDTH / 2 + (2 * 1.44 - 0.5) * calc200, calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "XXXXXXX", false);

	button1->SetNeighbourRight(button2);
	button2->SetNeighbourRight(button3);
	button3->SetNeighbourRight(button4);
	button4->SetNeighbourRight(button1);

	

	button1->SetAction([this, action,eq, item, itemMenu]() {
		if (action == "Equip") {
			gameScene->character->inventory->EquipWeapon(eq);
			gameScene->character->stats->IncreaseStats(eq->statIncrease);
		}
		else if (action == "Unequip") {
			gameScene->character->inventory->UnequipWeapon(eq);
			gameScene->character->stats->DecreaseStats(eq->statIncrease);
		}
		else if (action == "Use") {
			std::shared_ptr<Consumable> co = std::dynamic_pointer_cast<Consumable>(item);
			co->Consume();
			itemMenu->SetUpItems();
		}
		ButtonFunctionality::ChangeScene("itemMenu",false);
		});
	button2->SetAction([]() {});
	button3->SetAction([parentScene,this,itemMenu]() {

		auto& character = gameScene->character;
		auto& cellUnder = gameScene->character->currentCell;

		std::shared_ptr<Item> item = itemMenu->itemContainer->items[itemMenu->currentActiveButtonIndex];
		itemMenu->itemContainer->RemoveItem(itemMenu->currentActiveButtonIndex);
		if (std::shared_ptr<Equipment> eq = std::dynamic_pointer_cast<Equipment>(item); eq && eq->equipped) {
			itemMenu->itemContainer->UnequipWeapon(eq);
			gameScene->indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2, Resolution::HEIGHT / 2, 32, 32, "Unequipped", glm::vec3(1.0, 0.0, 0.0)));
		}
		item->BindItemToCell(gameScene->environment,gameScene,cellUnder);

		ButtonFunctionality::ChangeScene("itemMenu");
		});


	button4->SetAction([]() {});

	buttons.insert(std::make_pair(button1->id, button1));
	buttons.insert(std::make_pair(button2->id, button2));
	buttons.insert(std::make_pair(button3->id, button3));
	buttons.insert(std::make_pair(button4->id, button4));

	bool activeFound = false;
	for (const auto& pair : buttons)
	{
		if (pair.second->isActive) {
			if (!activeFound) {
				currentActiveButtonIndex = pair.first;
				activeFound = true;
			}
			else printf("Two active buttons in this scene!");
		}
	}
	
	menuBackground = std::make_shared<TexturedRect>(5 * Resolution::WIDTH / 7 - 544 / 2, Resolution::HEIGHT / 2 - 544 / 2 - 50, 544, 544, "itemSelect");
	
	std::string texture = item->inventoryTexture->texture;
	
	selectedItem = std::make_shared<TexturedRect>(5 * Resolution::WIDTH / 7 - 544 / 2 + 32, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - 50,
		LevelDataManagement::cellSize, LevelDataManagement::cellSize, texture);
	
	

	itemName = std::make_shared<Text>(1140, 450, 32, 32, item->GetLabel().c_str(), glm::vec3(0.5, 0.5, 0.5), Text::left);
	
	Scene::SetUp();
}

void ItemSelect::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	if (bl.GetButton(xpos, ypos) != -1) currentActiveButtonIndex = bl.GetButton(xpos, ypos);
	for (const auto& pair : buttons)
	{
		pair.second->shader->Bind();
		if (pair.first == currentActiveButtonIndex)
		{
			pair.second->shader->SetUniform1i("u_Inverter", 1);
		}
		else
		{
			pair.second->shader->SetUniform1i("u_Inverter", 0);
		}
	}
	gameScene->Update(xpos,ypos);
}

void ItemSelect::Draw(bool fromChild)
{
	if (parentScene) {
		parentScene->Draw(1);
	}
	for (const auto& pair : buttons)
	{
		pair.second->Draw();
	}
	//std::cout << "Pointer from ItemSelect side: " << parentScene->parentScene.get() << "\n";
	//std::cout << "Pointer from ItemSelect side: " << parentScene << "\n";
	menuBackground->Draw();
	selectedItem->Draw();
	gameScene->DrawFloatingIndicators();
	Scene::Draw();
	itemName->Draw();
}

void ItemSelect::PerformAction(int pressedButtonId)
{
	switch (pressedButtonId)
	{
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		buttons[currentActiveButtonIndex]->Activate();
		break;
	case GLFW_KEY_UP:
		if (buttons[currentActiveButtonIndex]->neighbours[0] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[0]->id;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (buttons[currentActiveButtonIndex]->neighbours[1] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[1]->id;
		}
		break;
	case GLFW_KEY_DOWN:
		if (buttons[currentActiveButtonIndex]->neighbours[2] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[2]->id;
		}
		break;
	case GLFW_KEY_LEFT:
		if (buttons[currentActiveButtonIndex]->neighbours[3] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[3]->id;
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
	case GLFW_KEY_X:
		ButtonFunctionality::ChangeScene("itemMenu", false);
		break;

	default:
		break;
	}
}

void ItemSelect::PerformActionScroll(double ypos)
{
}

void ItemSelect::ReceiveInfo(char* data)
{
}
