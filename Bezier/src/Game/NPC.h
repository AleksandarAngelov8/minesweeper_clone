#pragma once
#include "OpenGL/Mesh.h"
#include "LevelDataManagement/LevelDataManagement.h"
#include "OpenGL/Environment.h"
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL/DialogBox.h"
#include "Game/Quote.h"

class NPC : public std::enable_shared_from_this<NPC>
{
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	std::shared_ptr<DialogBox> dialogbox;
public:
	NPC(float spawnX, float spawnY, float width, float height,
		std::string textureName, unsigned int numOfSprites,
		std::shared_ptr<Scene> scene, std::vector<std::shared_ptr<Quote>> quotes);
	std::vector<std::shared_ptr<Quote>> quotes;
	std::unique_ptr<Shader> shader;
	void SetUpCellConnections();
	std::shared_ptr<Scene> scene;

	glm::vec2 currentSprite;
	std::array<std::shared_ptr<Cell>, 4> neighbours;
	std::shared_ptr<Cell> currentCell;
	glm::vec3 translation;
	unsigned int rotation;

	void Update();
	void Draw();

};

