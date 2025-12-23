#include "Arena.h"
#include "Server/Client.h"
#include "Server/Server.h"
#include "Game/Multiplayer/PlayerCharacter.h"
#include "Game/Multiplayer/PlayerEnemy.h"

#include <iostream>
#include <utility>
#include <future>
#include <stack>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#define user ButtonFunctionality::user
#define cellSize LevelDataManagement::cellSize
void Arena::SetUp(std::shared_ptr<Scene> parentScene)
{
	for (auto& row : sectors)
	{
		for (auto& cell : row)
		{
			cell.clear();
		}
	}
	if (user == 's') {
		character = std::make_shared<PlayerCharacter>(500, 100, 96, 128, "elysia3", glm::vec2(4, 4), shared_from_this(), 10, 10, 5, 1, 1);
		circle = std::make_shared<TexturedRect>(500, 100, 128, 128, "circle");
		enemy = std::make_shared<PlayerEnemy>(500, 500, 96, 128, "isadora", glm::vec2(4, 4), shared_from_this(), 10, 10, 5, 1, 1);
		currentPlayer = character;
	}
	else if (user == 'c') {
		enemy = std::make_shared<PlayerCharacter>(500, 100, 96, 128, "elysia3", glm::vec2(4, 4), shared_from_this(), 10, 10, 5, 1, 1);
		circle = std::make_shared<TexturedRect>(500, 500, 128, 128, "circle");
		character = std::make_shared<PlayerEnemy>(500, 500, 96, 128, "isadora", glm::vec2(4, 4), shared_from_this(), 10, 10, 5, 1, 1);
		currentPlayer = character;
	}
	character->arena = shared_from_this();
	enemy->arena = shared_from_this();

	timeUnits = 0;
	menu = nullptr;


	prevTimeStamp = std::chrono::steady_clock::now();


	Scene::SetUp();
}
void Arena::Synchronize(std::string command, std::vector<std::string> messageMap)
{
	std::ostringstream message;
	if (command == "") std::cerr << "Message map doesn't contain command!";
	message << command;

	bool first = true;
	for (const auto& value : messageMap) {
		if (!first) message << ",";
		first = false;
		message << value;
	}
	
	if (user == 's')
		Server::SendToClient(message.str().c_str());
	else if (user == 'c')
		Client::SendToServer(message.str().c_str());
}
void Arena::UpdateFloatingIndicators()
{
	for (int i = 0; i < indicators.size(); i++) {
		if (indicators[i]->shouldBeDeleted) {
			indicators.erase(indicators.begin() + i);
			continue;
		}
		indicators[i]->Update();
	}
}
void Arena::DrawFloatingIndicators()
{
	for (const auto& indicator : indicators) {
		indicator->Draw();
	}
}
void Arena::CleanupSectors() 
{
	if (!triggerCleanupSectors) return;
	for (auto& row : sectors) {
		for (auto& cell : row) {
			for (auto it = cell.begin(); it != cell.end(); ) {
				if (it->expired()) {
					it = cell.erase(it);
				}
				else {
					++it;
				}
			}
		}
	}
	triggerCleanupSectors = 0;
}
void Arena::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	if (menu != nullptr) {
		menu->Update(xpos, ypos);
		return;
	}
	mousePos = glm::vec2(xpos, ypos);
	currentTimeStamp = std::chrono::steady_clock::now();

	elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeStamp - prevTimeStamp);

	prevTimeStamp = currentTimeStamp;
	circle->shader->SetUniformVec3f("u_Color", glm::vec3(0.5+(sin(float(timeUnits)/50))/2, 0.5+(sin(float(timeUnits) / 50))/2, 0.5+(cos(float(timeUnits)/50))/2));
	character->Update(elapsedTime);
	//enemy->Update(elapsedTime);
	circle->translation = currentPlayer->GetTranslation() + glm::vec3(2,128/3,0);

	if (deathTrigger == 1 || deathTrigger == 2) {
		std::vector<std::string> messageMap;
		float speed = 10;
		currentPlayer->CreateMissile(mousePos, speed);
		messageMap.push_back(std::to_string(mousePos.x));
		messageMap.push_back(std::to_string(mousePos.y));
		messageMap.push_back(std::to_string(speed));
		Synchronize("m", messageMap);
	}

	//rework this make this into a separate function and just send stuff as a map or smth LMFAO
	
	std::vector<std::string> messageMap;
	messageMap.push_back(std::to_string(currentPlayer->GetTranslation().x));
	messageMap.push_back(std::to_string(currentPlayer->GetTranslation().y));
	Synchronize("c",messageMap);

	UpdateFloatingIndicators();

	CleanupSectors();

	//auto currentTime = std::chrono::steady_clock::now();
	//auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastClickTime);
	//std::cout << elapsedTime << "\n";
	

	timeUnits++;
}
void Arena::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();

	if (menu != nullptr)menu->Draw();

	character->Draw();
	enemy->Draw();
	circle->Draw();


	DrawFloatingIndicators();
	Scene::Draw();
}
void Arena::PerformAction(int pressedButtonId)
{
	if (menu != nullptr) {
		menu->PerformAction(pressedButtonId);
		if (pressedButtonId == GLFW_KEY_X) {
			menu = nullptr;
			ButtonFunctionality::shouldPerformButtonCheck = 1;
		}
		return;
	}
	float w;
	glm::vec3 tmp = currentPlayer->GetTranslation();
	std::vector<std::string> messageMap;
	float speed = 10;
	switch (pressedButtonId)
	{
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		tmp += glm::vec3(-4*elapsedTime.count()/10, 0, 0);
		//currentPlayer->Move(3, 4 * elapsedTime.count() / 10);
		currentPlayer->Move(tmp);
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		tmp += glm::vec3(4 * elapsedTime.count() / 10, 0, 0);
		//currentPlayer->Move(1, 4 * elapsedTime.count() / 10);
		currentPlayer->Move(tmp);
		break;
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN:
		tmp += glm::vec3(0, -4 * elapsedTime.count() / 10, 0);
		//currentPlayer->Move(2, 4 * elapsedTime.count() / 10);
		currentPlayer->Move(tmp);
		break;
	case GLFW_KEY_W:
	case GLFW_KEY_UP:
		tmp += glm::vec3(0, 4 * elapsedTime.count() / 10, 0);
		//currentPlayer->Move(0, 4 * elapsedTime.count() / 10);
		currentPlayer->Move(tmp);
		break;
	case GLFW_MOUSE_BUTTON_1:
	case GLFW_KEY_Z:
		currentPlayer->CreateMissile(mousePos,speed);
		messageMap.push_back(std::to_string(mousePos.x));
		messageMap.push_back(std::to_string(mousePos.y));
		messageMap.push_back(std::to_string(speed));
		Synchronize("m",messageMap);
		//Audio::PlaySound("interact");
		break;
	case GLFW_MOUSE_BUTTON_2:
	case GLFW_KEY_X:
		if (deathTrigger == 0) deathTrigger = 1;
		else deathTrigger = 0;
		//if (menu == nullptr) {
		//	ButtonFunctionality::ChangeScene("menu");
		//	ButtonFunctionality::shouldPerformButtonCheck = 0;
		//}
		//else menu = nullptr;
		break;
	case GLFW_KEY_C:
		
		break;
	case GLFW_KEY_V:
		break;
	case GLFW_KEY_B:
		indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2, Resolution::HEIGHT / 2, 64, 64, "+", glm::vec3(1.0, 1.0, 1.0)));
		break;
	case GLFW_KEY_LEFT_CONTROL:
		ButtonFunctionality::ActivateCursor();
		break;
	case GLFW_KEY_LEFT_SHIFT:
		ButtonFunctionality::DeactivateCursor();
		break;
	default:
		break;
	}
}
void Arena::PerformActionScroll(double ypos)
{
}
void Arena::ReceiveInfo(char* data)
{
	char controlChar = data[0];
	std::string dataS(data);
	int x,y;
	float s;
	switch (controlChar) {
	case 'c':
		dataS = dataS.substr(1);
		x = std::stoi(dataS.substr(0, dataS.find(",")));
		y = std::stoi(dataS.substr(dataS.find(",") + 1));
		//std::cout << x << "\t" << y << "\n";
		//std::cout << "x: " << enemy->GetTranslation().x << "\ty: " << enemy->GetTranslation().y << "\n";
		enemy->Move(glm::vec3(x, y, 0));
		break;
	case 'm':
		dataS = dataS.substr(1);
		x = std::stoi(dataS.substr(0, dataS.find(",")));
		dataS = dataS.substr(dataS.find(",")+1);
		y = std::stoi(dataS.substr(0, dataS.find(",")));
		s = std::stoi(dataS.substr(dataS.find(",") + 1));
		//std::cout << "x:" << x << "\ty:" << y << "\ts:" << s << "\n";
		enemy->CreateMissile(glm::vec2(x,y),s);
		break;
	}
}
