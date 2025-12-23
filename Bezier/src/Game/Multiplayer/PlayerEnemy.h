#pragma once
#include "Game/Multiplayer/Player.h"

class Missile;
class PlayerEnemy : public Player, public std::enable_shared_from_this<PlayerEnemy>
{

public:
	PlayerEnemy(float spawnX, float spawnY, float width, float height, std::string textureName, glm::vec2 numOfSprites, std::shared_ptr<Scene> scene, float maxhp, float currhp, float ad, unsigned int lvl, int exp);
	
	//std::chrono::steady_clock::time_point lastClickTime;

	std::vector<std::shared_ptr<Missile>> GetMissiles() override;
	void CreateMissile(glm::vec2 mousePos, float speed) override;
	int GetRotation() override;
	bool GetMoving(int i) override;
	void SetMoving(int i, bool v) override;
	void SetMoving(std::array<bool, 4> arr)  override;
	glm::vec3 GetSpriteCenterLocation() override;
	glm::vec3 GetTranslation()  override;
	void SetTranslation(glm::vec3 t)  override;
	void Move(glm::vec3 newPosition) override;
	void Move(int direction, float distance) override;

	std::chrono::steady_clock::time_point lastClickTime;
	void IterateAnimationSprite() override;
	void Update(std::chrono::milliseconds elapsedTime) override;
	void Draw() override;
	bool Rotate(int r) override;
	void DealDamage(std::shared_ptr<Player> player) override;
	void SetTranslation(int i, float v) override;
	glm::vec2 GetCurrentSprite() override;
	void CheckForIntersectionInSector(int yIndex, int xIndex) override;
	bool CheckIntersection(std::shared_ptr<Missile> missileB) override;
private:
	std::chrono::milliseconds animationSwapTime;

	glm::vec3 translation;
	std::array<bool, 4> moving;
	glm::vec2 currentSprite;
	float speed;
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	glm::vec2 numOfSprites;

};