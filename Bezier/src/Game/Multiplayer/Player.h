#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL/Mesh.h"
#include "LevelDataManagement/LevelDataManagement.h"
#include "Game/Stats.h"
#include <chrono>

class Arena;
class Missile;
class Player
{
public:
	std::shared_ptr<Arena> arena;
	std::vector<std::pair<glm::vec2,float>> missilesTrigger;
	std::vector<std::shared_ptr<Missile>> missiles;
	bool triggerChangeAnimationSprite;
	std::unique_ptr<Shader> shader;
	std::shared_ptr<Stats> stats;

	std::shared_ptr<Scene> scene;
	glm::vec3 prevTranslation;
	glm::vec3 differenceInPosition;
	int rotation;
	virtual void Dash() {};

	virtual glm::vec3 GetSpriteCenterLocation() { return glm::vec3(0.0, 0.0, 0.0); }
	virtual bool CheckIntersection(std::shared_ptr<Missile> missileB) { return false; }
	virtual void CheckForIntersectionInSector(int yIndex, int xIndex) {}
	virtual std::vector<std::shared_ptr<Missile>> GetMissiles() { return std::vector<std::shared_ptr<Missile>>(); };
	virtual void CreateMissile(glm::vec2 mousePos, float speed) {};
	virtual void Move(int direction, float distance) {};
	virtual void Move(glm::vec3 newPosition) {};
	virtual glm::vec2 GetCurrentSprite() { return glm::vec2(0.0f, 0.0f); };

	virtual int GetRotation() { return -5; };
	virtual bool GetMoving(int i) { return 0; };
	virtual void SetMoving(int i, bool v) {};
	virtual void SetMoving(std::array<bool,4> arr) {};
	virtual glm::vec3 GetTranslation() { return glm::vec3(); };
	virtual void SetTranslation(glm::vec3 t) {};
	virtual void SetTranslation(int i,float v) {};

	virtual void IterateAnimationSprite() {};
	virtual void Update(std::chrono::milliseconds elapsedTime) {};
	virtual void Draw() {};
	virtual bool Rotate(int r) { return -5; };
	virtual void DealDamage(std::shared_ptr<Player> player) {};
};

