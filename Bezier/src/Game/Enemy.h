#pragma once
#include "OpenGL/Mesh.h"
#include "LevelDataManagement/LevelDataManagement.h"
#include "OpenGL/Environment.h"
#include "glm/gtc/type_ptr.hpp"
#include "Game/Stats.h"
#include "Game/Character.h"
#include "Scene/Scene.h"
#include "Game/Bar.h"

class Enemy : public std::enable_shared_from_this<Enemy>
{
	float speed;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	long tickUnits;
	uint32_t direction;
public:
	std::shared_ptr<Character> character;
	int lockedInAnimation, turnsToAttack;
	std::string name;
	Enemy(float spawnX, float spawnY, float width, float height,
		std::string textureName, unsigned int numOfSprites, float maxhp, float ad, unsigned int level, std::shared_ptr<Scene> scene, bool alive, std::string name);
	void ChargeAttack();
	void Kill();
	void SetUpCellConnections();
	void Move();
	void Rotate(unsigned int r);
	void DamageCharacter();
	bool ShouldMove();
	void CreateHealthbar();
	std::shared_ptr<Bar> healthbar;
	std::pair<uint32_t, uint32_t> aggroed;
	bool movingLeft;
	bool movingRight;
	bool movingUp;
	bool movingDown;
	glm::vec2 target;
	std::shared_ptr<Scene> scene;
	std::unique_ptr<Shader> shader;
	std::shared_ptr<Stats> stats;
	glm::vec2 currentSprite;
	std::shared_ptr<Cell> currentCell;
	glm::vec3 translation;
	unsigned int rotation;
	bool alive;
	bool visible;

	void Update();
	void Draw();

	struct CompareEnemyY {
		bool operator()(const std::shared_ptr<Enemy>& e1, const std::shared_ptr<Enemy>& e2) {
			return e1->translation.y < e2->translation.y;
		}
	};
};

