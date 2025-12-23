#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL/Mesh.h"
#include "LevelDataManagement/LevelDataManagement.h"
#include "Game/Stats.h"
#include "ItemContainer.h"

class Character : public std::enable_shared_from_this<Character>
{

public:
	Character(float spawnX, float spawnY, float width, float height, std::string textureName, glm::vec2 numOfSprites, std::shared_ptr<Scene> scene, float maxhp, float currhp, float ad, unsigned int lvl, int exp);

	std::shared_ptr<Cell> attackingCell;

	void ChargeAttack();
	void InterruptAttack(std::shared_ptr<Cell> cell);
	void InterruptAttack();
	
	int range;
	int lockedInAnimation;
	int turnsToAttack;
	
	void IterateAnimationSprite();
	std::unique_ptr<Shader> shader;
	std::shared_ptr<Stats> stats;

	std::shared_ptr<ItemContainer> inventory;

	std::shared_ptr<Cell> currentCell;
	std::shared_ptr<Scene> scene;
	glm::vec3 translation;
	unsigned int rotation;
	bool movingLeft;
	bool movingRight;
	bool movingUp;
	bool movingDown;
	void Update();
	void Draw();
	void Stay();
	bool UpCollision();
	bool RightCollision();
	bool DownCollision();
	bool LeftCollision();
	void Rotate(unsigned int r);

	void Interact();
	void StartMoveAnimation(uint32_t direction);

	
private:
	int chargeAttack;
	glm::vec2 currentSprite;
	float speed;
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	glm::vec2 numOfSprites;
	unsigned int GetPosX(bool ceil);
	unsigned int GetPosY(bool ceil);
	void DealDamage(std::shared_ptr<Enemy> enemy);
	bool MovingCheck(uint32_t direction);
	void CheckForEnemies();
	

};