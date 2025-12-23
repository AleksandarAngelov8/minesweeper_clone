#pragma once
#include <vector>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>  
#include <Game/Cell.h>
#include <set>
#include <array>

#define NUMBER_OF_CELLS_X 100
#define NUMBER_OF_CELLS_Y 100

class Game;
class Scene;
class Edit;
class LevelDataManagement
{
public:
	static float cellSize;
	static const std::array<unsigned int, 8> zoomCellSizes;
	static std::vector<std::string> SplitString(const std::string& input, char delimiter);
	static void SaveGameState(std::shared_ptr<Game> gamestate);
	static void LoadGameState();
	static void EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, unsigned int texid, std::shared_ptr<Scene> scene);
	static void EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, std::shared_ptr<Scene> scene, std::string textureName, float maxHP, float AD, unsigned int level, std::string name);
	static void EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells, std::shared_ptr<Scene> scene, std::string textureName, std::vector<std::string> quotes, std::string name);
	static void EditLevelTextData(std::set<std::pair<unsigned int, unsigned int>> cells);
	static void EditLevelBinaryData(std::set<std::pair<unsigned int, unsigned int>> cells, unsigned int texid);
	static void ReadLevelBinaryData(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>>& cells);
	static void ReadLevelTextData(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>>& cells, std::shared_ptr<Scene> scene);
	static std::string ReadCellDataFromTextFile(std::set<std::pair<unsigned int, unsigned int>> cells, const char* file);
	static void CreateTextData();
	static void CreateBinaryData();
	static void ExportEnvironmentText(std::shared_ptr<Game> game, const char* name, std::set<std::pair<unsigned int, unsigned int>> path);
	static void LoadEnvironmentText(std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>>& cells, std::shared_ptr<Scene> scene, const char* name);
};

