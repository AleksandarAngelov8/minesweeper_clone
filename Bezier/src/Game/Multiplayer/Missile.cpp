#include "Missile.h"
#include "OpenGL/TexturedRect.h"
#include "OpenGL/Resolution.h"
#include "Audio/Audio.h"
#include "Scene/MultiplayerScenes/Arena.h"

void Missile::CheckForIntersectionInSector(std::pair<int, int> indices)
{
	for (const auto& missileTarget : arena->sectors[indices.first][indices.second]) {
		auto mt = missileTarget.lock(); if (!mt) continue;
		if (collisionCooldown.count(missileTarget) ||
			
			mt->collisionCooldown.count(shared_from_this()))
			continue;
		if (CheckIntersection(missileTarget))
		{
			//std::cout << "Intersection!!\n";
			Collide(missileTarget);
			mt->collisionCooldown.emplace(shared_from_this());
			mt->lastCollision = std::chrono::steady_clock::now();
			collisionCooldown.emplace(missileTarget);
			lastCollision = std::chrono::steady_clock::now();
		}
	}
}
void Missile::DeleteMissile() {
	shouldBeDeleted = 1;
	arena->triggerCleanupSectors = 1;
	auto& sectorSet = arena->sectors[sectorIndices.first][sectorIndices.second];
	std::erase_if(sectorSet,
		[](const std::weak_ptr<Missile>& w) { return w.expired(); });
}

void Missile::Bounce() {
	bounces++;
	if (bounces >= 5) {
		DeleteMissile();
	}
}

void Missile::Collide(std::weak_ptr<Missile> missileB)
{
	glm::vec2 delta = texture->translation - missileB.lock()->texture->translation;
	float distance = glm::length(delta);
	if (distance == 0.0f) return;
	glm::vec2 normal = delta / distance;
	
	glm::vec2 velA = direction * speed;
	glm::vec2 velB = missileB.lock()->direction * missileB.lock()->speed;


	glm::vec2 relativeVelocity = velA - velB;
	float velocityAlongNormal = glm::dot(relativeVelocity, normal);

	if (velocityAlongNormal > 0)
		return;

	float impulse = velocityAlongNormal;
	
	velA -= impulse * normal;
	velB += impulse * normal;

	// constant speed after collision
	//speed = glm::length(velA);
	//missileB.lock()->speed = glm::length(velB);

	if (speed > 0.0f)
		direction = glm::vec3(glm::normalize(velA),0.0f);
	if (missileB.lock()->speed > 0.0f)
		missileB.lock()->direction = glm::vec3(glm::normalize(velB),0.0f);

	Bounce();
}

bool Missile::CheckIntersection(std::weak_ptr<Missile> missileB)
{
	glm::vec2 delta = texture->translation - missileB.lock()->texture->translation;
	float distanceSquared = glm::dot(delta, delta);
	float radiusSum = 32 + 32;

	if (distanceSquared <= radiusSum * radiusSum) {
		return true;
	}
	return false;
}

std::pair<int, int> Missile::GetNextMissileSectorIndices(std::chrono::milliseconds elapsedTime, glm::vec3 nextPos)
{
	int tmpResX = Resolution::WIDTH, tmpResY = Resolution::HEIGHT;

	if (nextPos.x > tmpResX || nextPos.x < 0) {
		direction = glm::vec3(-direction.x, direction.y, 0.0);
		Bounce();
		//shouldBeDeleted = 1;
		//return;
		//Audio::PlaySound("interact");
	}
	if (nextPos.y > tmpResY || nextPos.y < 0) {
		direction = glm::vec3(direction.x, -direction.y, 0.0);
		Bounce();
		//shouldBeDeleted = 1;
		//return;
		//Audio::PlaySound("interact");
	}

	int ySectors = arena->sectors.size();
	int xSectors = arena->sectors[0].size();
	int yIndex = nextPos.y * ySectors / tmpResY;
	int xIndex = nextPos.x * xSectors / tmpResX;
	if (yIndex > ySectors - 1) yIndex = ySectors - 1;
	else if (yIndex < 0) yIndex = 0;
	if (xIndex > xSectors - 1) xIndex = xSectors - 1;
	else if (xIndex < 0) xIndex = 0;
	
	return std::make_pair(yIndex, xIndex);
}

Missile::Missile(glm::vec3 pos, glm::vec3 dir, float s, std::shared_ptr<Arena> arena, char prop):
	shouldBeDeleted(0), prop(prop), bounces(0)
{
	this->arena = arena;
	direction = dir;
	speed = s;
	texture = std::make_shared<TexturedRect>(pos.x,pos.y,32,32,"attack_1");
}

void Missile::Update(std::chrono::milliseconds elapsedTime)
{
	glm::vec3 nextPos = texture->translation + direction * (speed * elapsedTime.count() / 10);

	std::pair<int,int> nextIndices = GetNextMissileSectorIndices(elapsedTime, nextPos);

	
	if (nextIndices != sectorIndices) {
		arena->sectors[sectorIndices.first][sectorIndices.second].erase(shared_from_this());
	}
	auto timeSinceCollision = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastCollision);
	if (timeSinceCollision.count() > 50) {
		collisionCooldown.clear();
	}

	texture->translation = nextPos;
	if (arena->sectors[nextIndices.first][nextIndices.second].size() > 1) {
		CheckForIntersectionInSector(nextIndices);
	}
	
	arena->sectors[nextIndices.first][nextIndices.second].emplace(shared_from_this());

	sectorIndices = nextIndices;
	//for (const auto& [y,x] : arena->usedSectors) {
	//	std::cout << y << "-" << x << "\n";
	//}
	//std::cout << "\n";
	//std::cout << arena->usedSectors.size() << "\n";
	//std::cout << xIndex << "-" << yIndex << "\n";
	//texture->Update();
}

void Missile::Draw()
{
	texture->Draw();
}
