#include "Environment.h"
#include "Mesh.h"
#include "Scene/Game/Game.h"
#include <TextureInit/TextureInit.h>

void Environment::IterateWarningCell(std::pair<unsigned int, unsigned int> cellIndex, int warningIndex, std::string enemy, bool interrupt)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	std::shared_ptr<AttackWarning> aw = cell->attackWarnings[warningIndex+1][enemy];
	cell->attackWarnings[warningIndex+1].erase(enemy);
	if (warningIndex > 0 && !interrupt) {
		aw->warningIndex = warningIndex;
		auto p = cell->attackWarnings[warningIndex].emplace(enemy, aw);
	}
	
	int moreUrgentWarning = 0;
	for (int i = 1; i <= 3; i++) {
		if (!cell->attackWarnings[i].empty()) {
			moreUrgentWarning = i;
			break;
		}
	}
	if (!moreUrgentWarning) {
		cell->textureObject = nullptr;
		return;
	}
	cell->textureObject = std::make_shared<TexturedRect>(
		cell->position.first * LevelDataManagement::cellSize,
		cell->position.second * LevelDataManagement::cellSize,
		LevelDataManagement::cellSize,
		LevelDataManagement::cellSize,
		"warning_signal_" + std::to_string(moreUrgentWarning), true);
}

void Environment::BindWarningToCell(std::pair<unsigned int, unsigned int> cellIndex, std::string texture, std::function<void()> interraction, int warningIndex, std::string enemy)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	std::shared_ptr<TexturedRect> textureObject = std::make_shared<TexturedRect>(cell->position.first * LevelDataManagement::cellSize, cell->position.second * LevelDataManagement::cellSize, LevelDataManagement::cellSize, LevelDataManagement::cellSize, texture, true);

	cellsToDraw.emplace(cells[cellIndex.second][cellIndex.first]);

	cell->attackWarnings[warningIndex].emplace(enemy, std::make_shared<AttackWarning>(textureObject, warningIndex));
	
	for (int i = 1; i <= 3; i++) {
		if (!cell->attackWarnings[i].empty()) {
			cell->textureObject = std::make_shared<TexturedRect>(
				cell->position.first * LevelDataManagement::cellSize,
				cell->position.second * LevelDataManagement::cellSize,
				LevelDataManagement::cellSize,
				LevelDataManagement::cellSize,
				"warning_signal_" + std::to_string(i), true);
			return;
		}
	}
	cell->textureObject = textureObject;
}
void Environment::BindInteractableToCell(std::pair<unsigned int, unsigned int> cellIndex, std::string texture, std::function<void()> interraction, bool walkable)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	std::shared_ptr<TexturedRect> textureObject = std::make_shared<TexturedRect>(cell->position.first * LevelDataManagement::cellSize, cell->position.second * LevelDataManagement::cellSize, LevelDataManagement::cellSize, LevelDataManagement::cellSize, texture, true);

	cell->textureObject = textureObject;
	cell->walkable = walkable;

	cellsToDraw.emplace(cells[cellIndex.second][cellIndex.first]);

	cell->SetInteraction(interraction);
}
void Environment::BindInteractableToCell(std::pair<unsigned int, unsigned int> cellIndex, std::shared_ptr<Item> item, std::function<void()> interraction)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	item->floorTexture = std::make_shared<TexturedRect>(
		cell->position.first * LevelDataManagement::cellSize + LevelDataManagement::cellSize / 4,
		cell->position.second * LevelDataManagement::cellSize + LevelDataManagement::cellSize / 4
		, LevelDataManagement::cellSize / 2, LevelDataManagement::cellSize / 2, item->texture, true);
	cell->items.push(item);

	cellsToDraw.emplace(cells[cellIndex.second][cellIndex.first]);

	cell->SetInteraction(interraction);
}
void Environment::UnbindAllInteractableFromCell(std::pair<unsigned int, unsigned int> cellIndex)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	cell->items = std::stack<std::shared_ptr<Item>>();
	cellsToDraw.erase(cell);
	cell->SetInteraction([]() {},1);
}
void Environment::UnbindAllTexturesFromCell(std::pair<unsigned int, unsigned int> cellIndex)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	cell->textureObject = nullptr;
	cellsToDraw.erase(cell);
	cell->SetInteraction([]() {}, 1);
}
void Environment::UnbindTopItemFromCell(std::pair<unsigned int, unsigned int> cellIndex, std::shared_ptr<Game> gameScene)
{
	std::shared_ptr<Cell> cell = cells[cellIndex.second][cellIndex.first];
	cell->items.pop();
	if (cell->items.empty()){
		cellsToDraw.erase(cell)	;
		cell->SetInteraction([](){},1);
	}
	else {
		cell->items.top()->BindItemToCell(shared_from_this(), gameScene, cell);
		cell->items.pop();
	}
}
void Environment::ChangeCurrentVbo(std::map<std::pair<unsigned int, unsigned int>, unsigned int> indicesChecked, std::map<std::pair<unsigned int, unsigned int>, unsigned int> indicesVisible)
{
	std::vector<float> verts;
	for (const auto& index : indicesChecked) {
		float oppacity = 0.0;
		if (indicesVisible.count(index.first)) {
			uint32_t depth = indicesVisible[index.first];
			// increase the dividor to reduce visibility
			oppacity = (float)(6-depth)/6;
		}
		float texid = vboVertices[20 * (index.first.second * mapSize + index.first.first) + 4];
		//printf("%s",TextureInit::textureMapIToS[static_cast<unsigned int>(texid)].c_str());
		verts.push_back(LevelDataManagement::zoomCellSizes[4] * index.first.first);
		verts.push_back(LevelDataManagement::zoomCellSizes[4] * index.first.second);
		verts.push_back(0.0);
		verts.push_back(0.0);
		verts.push_back(texid);
		verts.push_back(oppacity);

		verts.push_back((LevelDataManagement::zoomCellSizes[4] * (index.first.first+1)));
		verts.push_back(LevelDataManagement::zoomCellSizes[4] * index.first.second);
		verts.push_back(1.0);
		verts.push_back(0.0);
		verts.push_back(texid);
		verts.push_back(oppacity);

		verts.push_back(LevelDataManagement::zoomCellSizes[4] * (index.first.first +1));
		verts.push_back(LevelDataManagement::zoomCellSizes[4] * (index.first.second +1));
		verts.push_back(1.0);
		verts.push_back(1.0);
		verts.push_back(texid);
		verts.push_back(oppacity);

		verts.push_back(LevelDataManagement::zoomCellSizes[4] * index.first.first);
		verts.push_back(LevelDataManagement::zoomCellSizes[4] * (index.first.second +1));
		verts.push_back(0.0);
		verts.push_back(1.0);
		verts.push_back(texid);
		verts.push_back(oppacity);
	}

	vao = std::make_unique<VertexArray>();
	vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
	VertexBufferLayout layout1;
	layout1.Push<float>(2);
	layout1.Push<float>(2);
	layout1.Push<float>(1);
	layout1.Push<float>(1);
	vbo->Bind();
	vao->AddBuffer(*vbo, layout1);
}
std::shared_ptr<Cell> Environment::GetCell(unsigned int x, unsigned int y)
{
	if (x < 0 || y < 0 || x >= cells.size() || y >= cells[x].size())
		return nullptr;	
	return cells[x][y];
}
bool Environment::CheckWalkable(unsigned int x, unsigned int y)
{
	if (x < 0 || y < 0 || x >= cells.size() || y >= cells[x].size())
		return false;
	return cells[x][y]->walkable;
}
//for Editor
Environment::Environment(std::shared_ptr<Scene> scene):
	translation(0.0, 0.0, 0.0)
{
	//LevelDataManagement::ReadLevelTextData(cells, scene);
	LevelDataManagement::LoadEnvironmentText(cells, scene,"testing1");
	
	std::vector<float> verts;
	for (const auto& y : cells) {
		for (const auto& x : cells[y.first]) {
			// 1
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.second));
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.first));
			verts.push_back(0.0);
			verts.push_back(0.0);
			verts.push_back(x.second->textureId);
			//2
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.second + 1));
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.first));
			verts.push_back(1.0);
			verts.push_back(0.0);
			verts.push_back(x.second->textureId);
			//3
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.second + 1));
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.first + 1));
			verts.push_back(1.0);
			verts.push_back(1.0);
			verts.push_back(x.second->textureId);
			//4
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.second));
			verts.push_back((int)(LevelDataManagement::zoomCellSizes[4]) * (x.second->position.first + 1));
			verts.push_back(0.0);
			verts.push_back(1.0);
			verts.push_back(x.second->textureId);
		}
	}
	
	std::vector<unsigned int> ind;
	for (int i = 0; i < verts.size()/20; i++) {
		ind.push_back(4 * i);
		ind.push_back(1 + 4 * i);
		ind.push_back(2 + 4 * i);
		ind.push_back(0 + 4 * i);
		ind.push_back(2 + 4 * i);
		ind.push_back(3 + 4 * i);
	}

	vao = std::make_unique<VertexArray>();
	vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
	VertexBufferLayout layout1;
	layout1.Push<float>(2);
	layout1.Push<float>(2);
	layout1.Push<float>(1);
	vbo->Bind();
	vao->AddBuffer(*vbo, layout1);
	ib = std::make_unique<IndexBuffer>(ind.data(), ind.size());

	shader = std::make_unique<Shader>("res/shaders/Game.shader");
	shader->Bind();
	shader->SetUniformVec1i("u_Texture", 25);
	//PROMENI TUK ------------------------^
	//shader->SetUniform1f("u_TexID", 0);
	translation += glm::vec3(0, 0, 0.0);
}
//for Game
Environment::Environment(std::shared_ptr<Scene> scene, std::shared_ptr<MapVisualization> mapv) :
	translation(0.0, 0.0, 0.0)
{
	//LevelDataManagement::ReadLevelTextData(cells, scene);
	if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
		game->enemies.clear();
	}
	for (unsigned int i = 0; i < mapv->mapSize; i++) {
		cells.insert({ i,std::map<unsigned int,std::shared_ptr<Cell>>() });
		for (unsigned int j = 0; j < mapv->mapSize; j++) {
			auto cell = std::make_shared<Cell>(j,i,mapv->walls[i][j]);
			cells[i].insert({j,cell});
			if (mapv->enemies.find({ i,j }) != mapv->enemies.end()) {
				if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
					std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(
						LevelDataManagement::zoomCellSizes[4] * i, 
						LevelDataManagement::zoomCellSizes[4] * j, 
						LevelDataManagement::zoomCellSizes[4], 
						LevelDataManagement::zoomCellSizes[4], 
						"slime",
						4, 
						20, 
						4, 
						1, 
						scene, 
						1, "enemy" + std::to_string(i) + "/" + std::to_string(j));
					game->enemies.emplace(("enemy"+std::to_string(i)+"/" + std::to_string(j)), enemy);
					cells[i][j]->enemy = enemy;
				}
			}
		}
	}
	for (int i = 0; i < cells.size(); i++)
	{
		for (int j = 0; j < cells[i].size(); j++)
		{
			for (int t = 1; t <= 3; t++) {
				cells[i][j]->attackWarnings.emplace(t,std::map<std::string,std::shared_ptr<AttackWarning>>());
			}
			std::shared_ptr<Cell> up;
			std::shared_ptr<Cell> right;
			std::shared_ptr<Cell> down;
			std::shared_ptr<Cell> left;
			if (i > 0) 
				down = cells[i-1][j];
			if (i < cells.size() - 1)
				up = cells[i+1][j];
			if (j > 0)
				left = cells[i][j-1];
			if (j < cells[i].size() - 1)
				right = cells[i][j+1];
			cells[i][j]->SetNeighbours(up,right,down,left);
		}
	}
	vao = mapv->vao;
	ib = mapv->ib;
	shader = mapv->shader;
	vboVertices = mapv->vertices;
	mapSize = mapv->mapSize;
	start = mapv->start;
	startDoor = mapv->startDoor;
	end = mapv->end;
	endDoor = mapv->endDoor;

	BindInteractableToCell(startDoor, "door", [scene, this]()
		{
			if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
				if (game->character->inventory->Contains("entranceKey")) {
					game->character->inventory->RemoveItem("entranceKey");
					auto cell = cells[startDoor.second][startDoor.first];
					cell->walkable = true;
					cell->textureObject->shouldBeDrawn = 0;
				}
			}
		});
	BindInteractableToCell(endDoor,"door", [scene, this]()
		{
			if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
				if (game->character->inventory->Contains("exitKey")) {
					game->character->inventory->RemoveItem("exitKey");
					auto cell = cells[endDoor.second][endDoor.first];
					cell->walkable = true;
					cell->textureObject->shouldBeDrawn = 0;

				}
			}
		});

	cells[end.second][end.first]->SetInteraction(
		[]() 
		{
			ButtonFunctionality::ChangeScene("game");
		},false);
	// make the chest here
	
}
void Environment::Update()
{
	for (const auto& cell : cellsToDraw) {
		if (cell->textureObject)cell->textureObject->Update();
		if (!cell->items.empty()) cell->items.top()->floorTexture->Update();
	}
}
void Environment::Draw()
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

	for (const auto& cell : cellsToDraw) {
		if (cell->textureObject)cell->textureObject->Draw();
		if (!cell->items.empty()) {
			cell->items.top()->floorTexture->Draw();
		}
	}
}
