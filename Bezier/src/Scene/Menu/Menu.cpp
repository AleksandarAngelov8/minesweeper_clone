#include "Menu.h"
#include <set>
#include "OpenGL/Resolution.h"
#include "Scene/MultiplayerScenes/Arena.h"

void Menu::SetUp(std::shared_ptr<Scene> parentScene)
{
	setup = 1;
	this->parentScene = parentScene;
	arenaScene = std::dynamic_pointer_cast<Arena>(parentScene);

	float calc200 = static_cast<float>(200.0 / 1920.0 * Resolution::WIDTH);
	float calc100 = static_cast<float>(100.0 / 1080 * Resolution::HEIGHT);
	
	std::shared_ptr<Button> button1 = std::make_shared<Button>(1, Resolution::WIDTH/2 + (0 * 1.44 - 0.5) * calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Items", true);
	std::shared_ptr<Button> button2 = std::make_shared<Button>(2, Resolution::WIDTH/2 + (1 * 1.44 - 0.5) * calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Save", false);
	std::shared_ptr<Button> button3 = std::make_shared<Button>(3, Resolution::WIDTH/2 + (2 * 1.44 - 0.5) * calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Load", false);
	std::shared_ptr<Button> button4 = std::make_shared<Button>(4, Resolution::WIDTH/2 + (3 * 1.44 - 0.5) * calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Main menu", false);

	button1->SetNeighbourRight(button2);
	button2->SetNeighbourRight(button3);
	button3->SetNeighbourRight(button4);
	button4->SetNeighbourRight(button1);

	button1->SetAction([]() {ButtonFunctionality::ChangeScene("itemMenu"); });
	//button2->SetAction([]() {LevelDataManagement::SaveGameState(std::dynamic_pointer_cast<Arena>(ButtonFunctionality::getCurrentSceneObject())); });
	//button3->SetAction([]() {LevelDataManagement::LoadGameState(); });
	button4->SetAction([]() {ButtonFunctionality::ChangeScene("mainMenu"); });

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
	dialogbox = std::make_shared<DialogBox>();
	dialogbox->SetUp();
	
	//info = std::make_shared<Text>(500, 64, 32, 32, "HELLO", glm::vec3(1.0, 1.0, 1.0), Text::centered);
	Scene::SetUp();
}
void Menu::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	//std::string text = std::to_string(xpos) +"/" + std::to_string(ypos);
	//dialogbox->SetText(text.c_str());
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
	arenaScene->Update(xpos,ypos);
}

void Menu::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();
	if (parentScene) {
		parentScene->Draw();
	}
	for (const auto& pair : buttons)
	{
		pair.second->Draw();
	}
	dialogbox->Draw();
	arenaScene->DrawFloatingIndicators();
	if (!fromChild) {
		Scene::Draw();
	}
}

void Menu::PerformAction(int pressedButtonId)
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
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		ButtonFunctionality::ChangeScene("arena",false);
		break;

	default:
		break;
	}
}

void Menu::PerformActionScroll(double ypos)
{
}

void Menu::ReceiveInfo(char* data)
{
}
