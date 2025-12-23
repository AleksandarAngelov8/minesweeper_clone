#pragma once
#include <memory>
#include "vendor/glm/vec3.hpp"
#include <chrono>
#include <set>

class Arena;
class TexturedRect;
class Missile : public std::enable_shared_from_this<Missile>
{
public:
	std::pair<int,int> sectorIndices;

	int bounces;
	char prop;
	bool shouldBeDeleted;
	void DeleteMissile();
	std::shared_ptr<Arena> arena;
	glm::vec3 previousPos;
	glm::vec3 direction;
	float speed;
	std::shared_ptr<TexturedRect> texture;
	std::chrono::steady_clock::time_point lastCollision;
	std::set<std::weak_ptr<Missile>, std::owner_less<std::weak_ptr<Missile>>> collisionCooldown;
	std::pair<int, int> GetNextMissileSectorIndices(std::chrono::milliseconds elapsedTime, glm::vec3 nextPos);
	void CheckForIntersectionInSector(std::pair<int,int> indices);
	void Collide(std::weak_ptr<Missile> missileB);
	bool CheckIntersection(std::weak_ptr<Missile> missileB);
	void Bounce();
	Missile(glm::vec3 pos, glm::vec3 dir, float s, std::shared_ptr<Arena> arena, char prop);
	void Update(std::chrono::milliseconds elapsedTime);
	void Draw();
};

