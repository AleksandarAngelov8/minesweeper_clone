#pragma once
#include <memory>
#include "glm/gtc/type_ptr.hpp"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "utility"
#include "map"
class MapVisualization
{
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexBuffer> vbo;
	
public:
	unsigned int seed;
	std::vector<float> vertices;
	std::map<std::pair<unsigned int, unsigned int>, bool> enemies;

	std::pair<unsigned int, unsigned int> start;
	std::pair<unsigned int, unsigned int> startDoor;
	std::pair<unsigned int, unsigned int> end;
	std::pair<unsigned int, unsigned int> endDoor;
	
	std::shared_ptr<VertexArray>  vao;
	std::shared_ptr<IndexBuffer>  ib;

	int mapSize;
	std::vector<std::vector<unsigned int>> walls;
	MapVisualization(unsigned int mapSize, long long seedOffSet);

	void SetEnemies(unsigned int count);
	void SetEnemy(int x, int y);
	void GenerateSeed(long long timeOffset);
	void SetupWalls();
	void AddNeededWalls(int iterations);
	void RemovedUnneededWalls(int iterations);
	void SetStartEnd();
	void CreateRoom(int xOffset, int yOffset, int range, bool doorFlag);
	void CreateStartEndRooms();
	bool CheckDoorability(std::pair<uint32_t, uint32_t> location, int wall);
	void Update();
	void Draw();
	std::shared_ptr<Shader> shader;
	glm::vec3 translation;
};

