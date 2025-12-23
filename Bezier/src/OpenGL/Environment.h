#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>  

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include "Game/Cell.h"
#include <LevelDataManagement/LevelDataManagement.h>
#include "Resolution.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "MapVisualization.h"
#include <OpenGL/TexturedRect.h>

class Scene;
class Environment: public std::enable_shared_from_this<Environment>
{
private:
	glm::mat4 mvp;
	std::unique_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexArray>  vao;
	std::shared_ptr<IndexBuffer>  ib;
	std::vector<float> vboVertices;
public:
	std::set<std::shared_ptr<Cell>> cellsToDraw;
	std::map<unsigned int, std::map<unsigned int, std::shared_ptr<Cell>>> cells;
	unsigned int mapSize;
	std::pair<unsigned int, unsigned int> start;
	std::pair<unsigned int, unsigned int> startDoor;
	std::pair<unsigned int, unsigned int> end;
	std::pair<unsigned int, unsigned int> endDoor;

	void IterateWarningCell(std::pair<unsigned int, unsigned int> cellIndex, int warningIndex, std::string enemy, bool interrupt = false);
	void BindWarningToCell(std::pair<unsigned int, unsigned int> cellIndex, std::string texture, std::function<void()> interraction, int warningIndex, std::string enemy);
	void BindInteractableToCell(std::pair<unsigned int, unsigned int> cellIndex, std::string texture, std::function<void()> interraction, bool walkable=false);
	void BindInteractableToCell(std::pair<unsigned int, unsigned int> cellIndex, std::shared_ptr<Item> item, std::function<void()> interraction);
	void UnbindAllInteractableFromCell(std::pair<unsigned int, unsigned int> cellIndex);
	void UnbindAllTexturesFromCell(std::pair<unsigned int, unsigned int> cellIndex);
	void UnbindTopItemFromCell(std::pair<unsigned int, unsigned int> cellIndex, std::shared_ptr<Game> gameScene);
	void ChangeCurrentVbo(std::map<std::pair<unsigned int, unsigned int>, unsigned int> indicesChecked, std::map<std::pair<unsigned int, unsigned int>, unsigned int> indicesVisible);
	std::shared_ptr<Cell> GetCell(unsigned int x, unsigned int y);
	bool CheckWalkable(unsigned int x, unsigned int y);
	std::shared_ptr<Shader> shader;
	glm::vec3 translation;
	Environment(std::shared_ptr<Scene> scene);
	Environment(std::shared_ptr<Scene> scene, std::shared_ptr<MapVisualization> mapv);
	void Update();
	void Draw();
};

