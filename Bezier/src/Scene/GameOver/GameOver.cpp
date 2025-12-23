#include "GameOver.h"
#include "OpenGL/Resolution.h"

void GameOver::SetUp(std::shared_ptr<Scene> parentScene)
{
	float calc200 = static_cast<float>(200.0 / 1920.0 * Resolution::WIDTH);
	float calc100 = static_cast<float>(100.0 / 990.0 * Resolution::HEIGHT);

	std::shared_ptr<Button> button1 = std::make_shared<Button>(1, 
		((Resolution::WIDTH / 2) - Resolution::WIDTH / 5), calc100 / 2, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Retry", true);
	
	std::shared_ptr<Button> button2 = std::make_shared<Button>(2, 
		((Resolution::WIDTH / 2) + Resolution::WIDTH / 5), calc100 / 2, calc200, calc100, glm::vec3(0.098, 0.165, 0.337), glm::vec3(1, 0.843, 0), "Main Menu", false);

	button1->SetNeighbourRight(button2);
	button2->SetNeighbourRight(button1);

	button1->SetAction([this]() {ButtonFunctionality::ChangeScene("game"); });
	button2->SetAction([this]() {ButtonFunctionality::ChangeScene("mainMenu"); });

	buttons.insert({ button1->id, button1 });
	buttons.insert({ button2->id, button2 });

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

	background = std::make_shared<TexturedRect>(0, 0, Resolution::WIDTH, Resolution::HEIGHT, "heca");

	Scene::SetUp();
}

void GameOver::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
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
}

void GameOver::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();

	background->Draw();
	for (const auto& pair : buttons)
	{
		pair.second->Draw();
	}
	Scene::Draw();
}

void GameOver::PerformAction(int pressedButtonId)
{
	switch (pressedButtonId)
	{
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

void GameOver::PerformActionScroll(double ypos)
{
}

void GameOver::ReceiveInfo(char* data)
{
}
