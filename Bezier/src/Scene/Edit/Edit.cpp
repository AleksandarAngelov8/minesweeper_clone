#include "Edit.h"
#include "ImGuiFunctionality/ImGuiFunctionality.h"
#include "Button/ButtonFunctionality.h"
#include "Scene/Game/Game.h"

#define cellSize LevelDataManagement::cellSize
void Edit::ResetEnvironment()
{
	enemies.clear();
	npcs.clear();
	environment = std::make_shared<Environment>(shared_from_this());
	ImGuiFunctionality::ReadCellData(shared_from_this());
}

void Edit::SelectCell(bool reset)
{
	if (reset) selectedCellsIndices.clear();
	selectedCellsIndices.insert({ (selectorSelecting->translation.x) / (int)(cellSize), (selectorSelecting->translation.y) / (int)(cellSize) });
	selectorSelected = std::make_shared<Selector>(selectedCellsIndices, glm::vec3(0.0, 1.0, 0.0));
}

void Edit::SelectCellsRectangle(bool reset, std::vector<std::pair<unsigned int, unsigned int>> cells)
{
	if (reset) selectedCellsIndices.clear();
	for (const auto& cell : cells) {
		selectedCellsIndices.insert({ cell.first, cell.second});
	}
	selectorSelected = std::make_shared<Selector>(selectedCellsIndices, glm::vec3(0.0, 1.0, 0.0));
	ImGuiFunctionality::ReadCellData(shared_from_this());
}

void Edit::SetUp(std::shared_ptr<Scene> parentScene)
{
	Scene::SetUp();
	try {
		//LevelDataManagement::LoadEnvironmentText(shared_from_this(), "testin1");
		environment = std::make_shared<Environment>(shared_from_this());
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred: " << e.what() << std::endl;
	}

	setup = 1;
	zoomIndex = 4;
	startRectangle = { 1.0,1.0 };
	selectingRectangle = false;
	selectorSelecting = std::make_shared<Selector>(0, 0, (int)(cellSize), (int)(cellSize), glm::vec3(1.0, 0.0, 1.0));
	selectorSelected = std::make_shared<Selector>(selectedCellsIndices , glm::vec3(0.0, 1.0, 0.0));
	Mesh::scaleCamera = glm::vec3(1.0, 1.0, 0.0);

}

void Edit::Update(float xpos, float ypos)
{
	Scene::Update(xpos, ypos);
	//cellSize = cellSize * Mesh::scaleCamera.x;
	cellSize = static_cast<float>(LevelDataManagement::zoomCellSizes[zoomIndex]);

	mouseLocation = std::make_pair(xpos , ypos);
	if (!haltSelecting) {
		unsigned int x = static_cast<unsigned int>(mouseLocation.first + cellSize) / cellSize;
		unsigned int y = static_cast<unsigned int>(mouseLocation.second + cellSize) / cellSize;
		currentCell = std::make_pair(x - 1, y - 1);
		selectorSelecting->translation = glm::vec3(x * (int)(cellSize) - (int)(cellSize)/2, y * (int)(cellSize) - (int)(cellSize)/2, 0);
		//printf("%i\n", (int)(cellSize));
	}
	if (selectingRectangle) {
		rectangleSelector->ScaleX(xpos-startRectangle.first);
		rectangleSelector->ScaleY(ypos-startRectangle.second);
	}
	if (!ButtonFunctionality::CheckPressedMouseButton(GLFW_MOUSE_BUTTON_1) && selectingRectangle) {
		//printf("%f - %f\t", xpos, ypos);
		//printf("%f - %f\n", startRectangle.first, startRectangle.second);
		selectingRectangle = false;
		if (xpos > (int)(cellSize) * NUMBER_OF_CELLS_X || startRectangle.first > (int)(cellSize) * NUMBER_OF_CELLS_X
			|| ypos > (int)(cellSize) * NUMBER_OF_CELLS_Y || startRectangle.second > (int)(cellSize) * NUMBER_OF_CELLS_Y) {
		}
		else {
			std::vector<std::pair<unsigned int, unsigned int>> cellsInside = std::vector<std::pair<unsigned int, unsigned int>>();
			if (xpos > startRectangle.first) {
				for (int i = ((int)startRectangle.first / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize)/2; i < ((int)xpos / (int)(cellSize)) * (int)(cellSize); i += (int)(cellSize)) {
					if (ypos > startRectangle.second) {
						for (int j = ((int)startRectangle.second / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize)/2; j < ((int)ypos / (int)(cellSize)) * (int)(cellSize); j += (int)(cellSize)) {
							cellsInside.push_back(std::make_pair((Mesh::cameraTranslate.x/(cellSize)) + (i + (int)(cellSize)) / (int)(cellSize), (Mesh::cameraTranslate.y / (cellSize)) + (j + (int)(cellSize)) / (int)(cellSize)));
						}
					}
					else {
						for (int j = ((int)ypos / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize)/2; j < ((int)startRectangle.second / (int)(cellSize)) * (int)(cellSize); j += (int)(cellSize)) {
							cellsInside.push_back(std::make_pair((Mesh::cameraTranslate.x / (cellSize)) + (i + (int)(cellSize)) / (int)(cellSize), (Mesh::cameraTranslate.y / (cellSize)) + (j + (int)(cellSize)) / (int)(cellSize)));
						}
					}
				}
			}
			else {
				for (int i = ((int)xpos / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize)/2; i < ((int)startRectangle.first / (int)(cellSize)) * (int)(cellSize); i += (int)(cellSize)) {
					if (ypos > startRectangle.second) {
						for (int j = ((int)startRectangle.second / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize)/2; j < ((int)ypos / (int)(cellSize)) * (int)(cellSize); j += (int)(cellSize)) {
							cellsInside.push_back(std::make_pair((Mesh::cameraTranslate.x / (cellSize)) + (i + (int)(cellSize)) / (int)(cellSize), (Mesh::cameraTranslate.y / (cellSize)) + (j + (int)(cellSize)) / (int)(cellSize)));
						}
					}
					else {
						for (int j = ((int)ypos / (int)(cellSize)) * (int)(cellSize) - (int)(cellSize) / 2; j < ((int)startRectangle.second / (int)(cellSize)) * (int)(cellSize); j += (int)(cellSize)) {
							cellsInside.push_back(std::make_pair((Mesh::cameraTranslate.x / (cellSize)) + (i + (int)(cellSize)) / (int)(cellSize), (Mesh::cameraTranslate.y / (cellSize)) + (j + (int)(cellSize)) / (int)(cellSize)));
						}
					}
				}
			}

			for (const auto& n : cellsInside) {
				printf("%i/%i\n", n.first, n.second);
			}
			SelectCellsRectangle((!ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_LEFT_SHIFT)), cellsInside);
		}
	}
	if (resetSelected) {
		resetSelected = 0;
		selectorSelected = std::make_shared<Selector>(selectedCellsIndices, glm::vec3(0.0, 1.0, 0.0));
	}
	//printf("%f\n", Mesh::scaleCamera.x);
}

void Edit::Draw(bool fromChild)
{
	Renderer renderer;
	renderer.Clear();

	environment->Draw();
	selectorSelecting->Draw();
	selectorSelected->Draw();
	
	if (selectingRectangle) rectangleSelector->Draw();
	for (const auto& enemy : enemies) {
		enemy.second->Draw();
	}
	for (const auto& npc : npcs) {
		npc.second->Draw();
	}
	Scene::Draw();
}

void Edit::PerformAction(int pressedButtonId)
{
	switch (pressedButtonId)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (haltSelecting) return;
		if (!selectingRectangle) startRectangle = std::make_pair(mouseLocation.first,mouseLocation.second);
		selectingRectangle = true;
		rectangleSelector = std::make_shared<Mesh>(mouseLocation.first + Mesh::cameraTranslate.x,mouseLocation.second + Mesh::cameraTranslate.y, 1, 1, glm::vec3(0.0, 0.7, 0.0));

		//if (haltSelecting) return;
		//if (ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_LEFT_SHIFT)) SelectCell(false);
		//else SelectCell(true);
		
		break;
	case GLFW_KEY_LEFT:
		Mesh::MoveCamera(std::min(- static_cast<float>(LevelDataManagement::zoomCellSizes[4]),-cellSize), 0);
		break;
	case GLFW_KEY_RIGHT:
		Mesh::MoveCamera(std::max(static_cast<float>(LevelDataManagement::zoomCellSizes[4]), cellSize), 0);
		break;
	case GLFW_KEY_DOWN:
		Mesh::MoveCamera(0, std::min(-static_cast<float>(LevelDataManagement::zoomCellSizes[4]), -cellSize));
		break;
	case GLFW_KEY_UP:
		Mesh::MoveCamera(0, std::max(static_cast<float>(LevelDataManagement::zoomCellSizes[4]), cellSize));
		break;
	case GLFW_KEY_Z:
		Mesh::cameraTranslate = glm::vec3(0.0,0.0,0.0);
		/*for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				printf("%f\t", Mesh::View[i][j]);
			}
			printf("\n");
		}*/
		break;
		
	default:
		break;
	}
	resetSelected = 1;
}

void Edit::PerformActionScroll(double ypos)
{
	printf("%f",ypos);
	if (ypos < 0 && zoomIndex == 0) return;
	if (ypos > 0 && zoomIndex == 4) return;
	zoomIndex += ypos/abs(ypos);
	Mesh::scaleCamera = glm::vec3(
		abs(1.0) * LevelDataManagement::zoomCellSizes[zoomIndex] / LevelDataManagement::zoomCellSizes[4]
		, abs(1.0) * LevelDataManagement::zoomCellSizes[zoomIndex] / LevelDataManagement::zoomCellSizes[4], 0.0);

	resetSelected = 1;
}

void Edit::ReceiveInfo(char* data)
{
}
