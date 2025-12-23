#include "MapVisualization.h"
#include "OpenGL/Mesh.h"
#include "time.h"
#include "TextureInit/TextureInit.h"
#include <iostream>
#include <chrono>
#include <LevelDataManagement/LevelDataManagement.h>

#define wallsize LevelDataManagement::cellSize

MapVisualization::MapVisualization(unsigned int mapSize, long long seedOffSet):
	scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), mapSize(mapSize)
{
	GenerateSeed(seedOffSet);
	SetupWalls();

	for (int i = 0; i < 2; i++) {
		AddNeededWalls(3-i);
		RemovedUnneededWalls(1);
	}
	
	SetStartEnd();
	
	SetEnemies(100);
	//SetEnemy(93,49);
	//SetEnemy(93, 50);
	//SetEnemy(93,51);

	unsigned int texId = TextureInit::textureMapSToI["dungeonWall1"];
	std::vector<float> verts;
	for (int i = 0; i < walls.size(); i++) {
		for (int j = 0; j < walls[i].size(); j++) {
			texId = walls[i][j];
			if (walls[i][j] == TextureInit::textureMapSToI["dungeonWall1"] && 
				i > 1 &&
				walls[i - 1][j] == TextureInit::textureMapSToI["dungeonGround"]) {

				texId = TextureInit::textureMapSToI["dungeonWall2"];
				walls[i][j] = texId;
			}
			if (i == start.second && j == start.first) {
				texId = TextureInit::textureMapSToI["dungeonUpstairs"];
			}
			if (i == end.second && j == end.first) {
				texId = TextureInit::textureMapSToI["dungeonDownstairs"];
			}
			verts.push_back((int)(wallsize * (i)));
			verts.push_back((int)(wallsize * (j)));
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			verts.push_back((texId));
			//2
			verts.push_back((int)(wallsize) * (i + 1));
			verts.push_back((int)(wallsize) * (j));
			verts.push_back(1.0f);
			verts.push_back(0.0f);
			verts.push_back((texId));
			//3
			verts.push_back((int)(wallsize) * (i + 1));
			verts.push_back((int)(wallsize) * (j + 1));
			verts.push_back(1.0f);
			verts.push_back(1.0f);
			verts.push_back((texId));
			//4
			verts.push_back((int)(wallsize) * (i));
			verts.push_back((int)(wallsize) * (j + 1));
			verts.push_back(0.0f);
			verts.push_back(1.0f);
			verts.push_back((texId));
		}
	}
	vertices = verts;
	std::vector<unsigned int> ind;
	for (int i = 0; i < verts.size() / 20; i++) {
		ind.push_back(4 * i);
		ind.push_back(1 + 4 * i);
		ind.push_back(2 + 4 * i);
		ind.push_back(0 + 4 * i);
		ind.push_back(2 + 4 * i);
		ind.push_back(3 + 4 * i);
	}

	//for (float i : verts) {
	//	printf("%.2f\t",i);
	//}

	vao = std::make_unique<VertexArray>();
	vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
	VertexBufferLayout layout1;
	layout1.Push<float>(2);
	layout1.Push<float>(2);
	layout1.Push<float>(1);
	vbo->Bind();
	vao->AddBuffer(*vbo, layout1);
	ib = std::make_unique<IndexBuffer>(ind.data(), ind.size());

	shader = std::make_unique<Shader>("res/shaders/Environment.shader");
	//PROMENI TUK ------------------------^
	shader->Bind();
	shader->SetUniformVec1i("u_Texture", 25);
	//shader->SetUniform1f("u_TexID", 4); 
	translation += glm::vec3(0, 0, 0.0);
}
void MapVisualization::GenerateSeed(long long timeOffset) {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	seed = std::hash<long long>{}(millis + timeOffset);
	seed = 2449738198;
	srand(seed);
}

void MapVisualization::SetEnemies(unsigned int count)
{
	for (int i = 0; i < count; i++) {
		std::pair<unsigned int, unsigned int> pos = { 1,1 };
		do {
			pos = { rand() % mapSize,rand() % mapSize };
		} while ((walls[pos.first][pos.second] == TextureInit::textureMapSToI["dungeonWall1"]) || enemies.find(pos) != enemies.end());
		enemies.emplace(pos, true);
	}
}
void MapVisualization::SetEnemy(int x, int y)
{
	if (walls[y][x] == TextureInit::textureMapSToI["dungeonWall1"]) {
		std::cout << "Enemy cannot be created";
		return;
	}
	std::pair<unsigned int, unsigned int> pos = { y,x};
	enemies.emplace(pos, true);
}

void MapVisualization::SetupWalls()
{
	srand(seed);

	for (int i = 0; i < mapSize; i++) {
		walls.push_back(std::vector<unsigned int>());
		for (int j = 0; j < mapSize; j++) {
			if (i == 0 || i == mapSize - 1 || j == 0 || j == mapSize - 1) walls[i].push_back(TextureInit::textureMapSToI["dungeonWall1"]);
			else {
				if (rand() % 3 == 0)
					walls[i].push_back(TextureInit::textureMapSToI["dungeonWall1"]);
				else walls[i].push_back(TextureInit::textureMapSToI["dungeonGround"]);
			}
		}
	}
	
}

void MapVisualization::Update()
{
}

void MapVisualization::Draw()
{
	Renderer renderer;

	glm::mat4 model(1.0f);
	model = glm::translate(model, translation);
	model = glm::translate(model, -Mesh::cameraTranslate);
	model = glm::scale(model, Mesh::scaleCamera);
	mvp = Mesh::Proj * Mesh::View * model;
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);

	renderer.Draw(*vao, *ib, *shader);
}

void MapVisualization::AddNeededWalls(int iterations) {
	for (int t = 0; t < iterations; t++) {
		std::vector<std::vector<unsigned int>> tmpWalls = walls;

		for (int i = 0; i < mapSize; i++) {
			for (int j = 0; j < mapSize; j++) {
				if (i == 0 || i == mapSize - 1 || j == 0 || j == mapSize - 1) tmpWalls[i][j] = TextureInit::textureMapSToI["dungeonWall1"];
				else {
					int chance = 
						(walls[i - 1][j] == TextureInit::textureMapSToI["dungeonWall1"]) + 
						(walls[i + 1][j] == TextureInit::textureMapSToI["dungeonWall1"]) + 
						(walls[i][j - 1] == TextureInit::textureMapSToI["dungeonWall1"]) + 
						(walls[i][j + 1] == TextureInit::textureMapSToI["dungeonWall1"]);

					if (walls[i][j] == TextureInit::textureMapSToI["dungeonWall1"] || (chance > 2)) {
						tmpWalls[i][j] = TextureInit::textureMapSToI["dungeonWall1"];
					}
				}
			}
		}
		walls = tmpWalls;
	}
}

void MapVisualization::RemovedUnneededWalls(int iterations) {
	for (int t = 0; t < iterations; t++) {
		std::vector<std::vector<unsigned int>> tmpWalls = walls;
		for (int i = 0; i < mapSize; i++) {
			for (int j = 0; j < mapSize; j++) {
				if (i == 0 || i == mapSize - 1 || j == 0 || j == mapSize - 1) tmpWalls[i][j] = TextureInit::textureMapSToI["dungeonWall1"];
				else {
					int chance =
						(walls[i - 1][j] == TextureInit::textureMapSToI["dungeonWall1"]) +
						(walls[i + 1][j] == TextureInit::textureMapSToI["dungeonWall1"]) +
						(walls[i][j - 1] == TextureInit::textureMapSToI["dungeonWall1"]) +
						(walls[i][j + 1] == TextureInit::textureMapSToI["dungeonWall1"]);

					if (walls[i][j] == TextureInit::textureMapSToI["dungeonWall1"] && (chance < 2)) 
						tmpWalls[i][j] = TextureInit::textureMapSToI["dungeonGround"];
				}
			}
		}
		walls = tmpWalls;
	}
}

void MapVisualization::SetStartEnd()
{
	start = {1,1};
	end = {mapSize-2,mapSize-2};
	do {
		if (walls[start.second][start.first] == TextureInit::textureMapSToI["dungeonWall1"]) start = {rand() % mapSize,rand() % mapSize};
		if (walls[end.second][end.first] == TextureInit::textureMapSToI["dungeonWall1"]) end = { rand() % mapSize,rand() % mapSize };
	} while ((walls[start.second][start.first] == TextureInit::textureMapSToI["dungeonWall1"]) ||
		(walls[end.second][end.first] == TextureInit::textureMapSToI["dungeonWall1"]));
	
	CreateStartEndRooms();
}

void MapVisualization::CreateRoom(int xOffset, int yOffset, int range, bool doorFlag) {
	for (int i = -range; i < range; i++) {
	skipInnerLoop:
		for (int j = -range; j < range; j++) {
			int y = i + yOffset, x = j + xOffset;
			if (y < 1) {
				i++;
				goto skipInnerLoop;
			}
			if (y >= walls.size() - 1) return;
			if (x < 1) continue;
			if (x >= walls.size() - 1) break;
			if (i == -range || i == range - 1 || j == -range || j == range - 1) {
				walls[y][x] = TextureInit::textureMapSToI["dungeonWall1"];
				int wall = 0;
				if (abs(i) == abs(j));
				else if (j == -range || j == range - 1) wall = 1;
				else if (i == -range || i == range - 1) wall = 2;
				if (!doorFlag && CheckDoorability({ y, x }, wall)) {
					if (range == 5) startDoor = { x,y };
					if (range == 3) endDoor = { x,y };
					doorFlag = true;
					walls[y][x] = TextureInit::textureMapSToI["dungeonGround"];
				}
			}
			else {
				walls[y][x] = TextureInit::textureMapSToI["dungeonGround"];
			}
		}
	}
}
void MapVisualization::CreateStartEndRooms() {
	bool door = false;

	CreateRoom(start.first, start.second, 5, door);

	door = false;

	CreateRoom(end.first, end.second, 3, door);
}
bool MapVisualization::CheckDoorability(std::pair<uint32_t, uint32_t> location, int wall)//wall = 0 is a corner, wall = 1 left-right wall, wall = 2 up-down wall
{
	if (location.first - 1 < 0 || location.first + 1 >= walls.size() ||
		location.second - 1 < 0 || location.second + 1 >= walls.size()) return false;

	int left = location.second - 1, right = location.second + 1, up = location.first + 1, down = location.first - 1;

	if (walls[location.first][left] == TextureInit::textureMapSToI["dungeonGround"] &&
		walls[location.first][right] == TextureInit::textureMapSToI["dungeonGround"] && wall == 1 ||

		walls[down][location.second] == TextureInit::textureMapSToI["dungeonGround"] &&
		walls[up][location.second] == TextureInit::textureMapSToI["dungeonGround"] && wall == 2
		) return true;
	return false;
}

