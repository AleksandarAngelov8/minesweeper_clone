#pragma once
#include "Scene/Scene.h"
#include "OpenGL/Environment.h"
#include "OpenGL/Selector.h"
#include <set>
#include "OpenGL/MapVisualization.h"

class Edit : public Scene, public std::enable_shared_from_this<Edit>
{
	std::shared_ptr<Selector> selectorSelecting;
	std::shared_ptr<Selector> selectorSelected;
	std::shared_ptr<Mesh> rectangleSelector;

	std::pair<float, float> startRectangle;
	std::pair<float, float> mouseLocation;
	bool selectingRectangle;
	bool resetSelected;
public:
	std::map<std::string, std::shared_ptr<Enemy>> enemies;
	std::map<std::string, std::shared_ptr<NPC>> npcs;
	std::shared_ptr<Environment> environment;
	unsigned int zoomIndex;
	std::set<std::pair<unsigned int, unsigned int>> selectedCellsIndices;
	void ResetEnvironment();
	bool haltSelecting;
	void SelectCell(bool reset);
	void SelectCellsRectangle(bool reset, std::vector<std::pair<unsigned int, unsigned int>> cells);
	std::pair<unsigned int, unsigned int> currentCell;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

