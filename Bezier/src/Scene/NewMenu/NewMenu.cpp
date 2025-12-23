#include <set>
#include "NewMenu.h"
#include "OpenGL/Resolution.h"
#include "stdlib.h"
#include "Button/ButtonFunctionality.h"

void NewMenu::SetUp(std::shared_ptr<Scene> parentScene) 
{
	setup = 1;
	float calc200 = static_cast<float>(200.0 / 1920.0 * Resolution::WIDTH);
	float calc100 = static_cast<float>(100.0 / 1080 * Resolution::HEIGHT);

	//std::shared_ptr<Button> button1 = std::make_shared<Button>(1, Resolution::WIDTH - (0), Resolution::HEIGHT - calc100,					calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "New Game", true, Resolution::WIDTH / 1920.0f);
	//std::shared_ptr<Button> button2 = std::make_shared<Button>(2, Resolution::WIDTH + (0*1.25 - 0.5)*calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Continue", true, Resolution::WIDTH / 1920.0f);
	//std::shared_ptr<Button> button3 = std::make_shared<Button>(3, Resolution::WIDTH + (1*1.25 - 0.5)*calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Edit", false, Resolution::WIDTH / 1920.0f);
	//std::shared_ptr<Button> button4 = std::make_shared<Button>(4, Resolution::WIDTH + (2*1.25 - 0.5)*calc200, Resolution::HEIGHT - calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Exit", false, Resolution::WIDTH / 1920.0f);
	float x = (Resolution::WIDTH - calc200 / 2) - calc200 / 2;
	std::shared_ptr<Button> button1 = std::make_shared<Button>(1, x, (4*1.25 - 0.5) * calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "New Game", true);
	std::shared_ptr<Button> button2 = std::make_shared<Button>(2, x, (3*1.25 - 0.5) * calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Continue", false);
	std::shared_ptr<Button> button3 = std::make_shared<Button>(3, x, (2*1.25 - 0.5) * calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Edit", false);
	std::shared_ptr<Button> button4 = std::make_shared<Button>(4, x, (1*1.25 - 0.5) * calc100, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Exit", false);


	printf("x = %.2f\ty = %.2f\n", (Resolution::WIDTH - calc200 / 2) - calc200 / 2, (4*1.25 - 0.5) * calc100);
	printf("x = %.2f\ty = %.2f\n", (Resolution::WIDTH - calc200 / 2) - calc200 / 2, (3*1.25 - 0.5) * calc100);
	printf("x = %.2f\ty = %.2f\n", (Resolution::WIDTH - calc200 / 2) - calc200 / 2, (2*1.25 - 0.5) * calc100);
	printf("x = %.2f\ty = %.2f\n", (Resolution::WIDTH - calc200 / 2) - calc200 / 2, (1*1.25 - 0.5) * calc100);

	button1->SetNeighbourDown(button2);
	button2->SetNeighbourDown(button3);
	button3->SetNeighbourDown(button4);
	button4->SetNeighbourDown(button1);

	button1->SetAction([this]() {ButtonFunctionality::ChangeScene("minefield"); });
	//button2->SetAction([this]() {LevelDataManagement::LoadGameState(); });
	//button3->SetAction([this]() {ButtonFunctionality::ChangeScene("edit"); });
	button4->SetAction([this]() {ButtonFunctionality::CloseWindow(); });

	buttons.insert({ button1->id, button1 });
	buttons.insert({ button2->id, button2 });
	buttons.insert({ button3->id, button3 });
	buttons.insert({ button4->id, button4 });

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
	Mesh::cameraTranslate = glm::vec3(0.0, 0.0, 0.0);

	background = std::make_shared<TexturedRect>(0,0,Resolution::WIDTH,Resolution::HEIGHT,"text");
	ButtonFunctionality::shouldPerformButtonCheck = 0;

	text = std::make_shared<Text>(0, 500, 32, 32, "* Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ullamcorper est ante, non pulvinar tortor fermentum quis.",glm::vec3(1,0.5,1),Text::left);
	Scene::SetUp();
}

void NewMenu::Update(float xpos, float ypos)
{
	Scene::Update(xpos,ypos);
	for (const auto& pair: buttons)
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
}

void NewMenu::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();

	//background->Draw();
	for (const auto& pair: buttons)
	{
		pair.second->Draw();
	}
	text->Draw();
	Scene::Draw();
}

void NewMenu::PerformAction(int pressedButtonId)
{
	if (currentActiveButtonIndex == -1) return;
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
	default:
		break;
	}
}

void NewMenu::PerformActionScroll(double ypos)
{
}

void NewMenu::ReceiveInfo(char* data)
{
}
