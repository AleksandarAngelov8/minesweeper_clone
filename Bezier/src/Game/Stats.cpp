#include "Stats.h"
#include "cmath"
#include "Equipment.h"
#include "Character.h"
#include "Scene/MultiplayerScenes/Arena.h"

void Stats::CalculateAttackDamage() {

	attackDamage = (float)baseAttackDamage * (1 + level * 0.05);
}

void  Stats::CalculateMaxHealth() {
	maxHealth = (float)baseHealth * std::pow(1.3, level);
	currentHealth = maxHealth;
}

 int Stats::ExpToLevelUp() {
	return std::ceil(50 + 2 * std::pow(level - 1, std::exp(1)));
}

 int Stats::Divisor( int level) {
	return 2 + (level - 1) * (50 - 2) / (25 - 1);
}

 int Stats::ExpFromEnemy(int charLevel) {
	float levelDifference = level - charLevel;
	int baseExp = 20;
	if (levelDifference > 0) {
		return baseExp * std::pow(1.2, levelDifference);
	}
	else if (levelDifference == 0) {
		return baseExp;
	}
	else {
		return baseExp * std::pow(0.8, -levelDifference);
	}
}

 Stats::Stats(int mh, int curhp, int ad, int lv, int exp, std::shared_ptr<Arena> gameScene)
	 : baseHealth(mh), currentHealth(curhp), baseAttackDamage(ad), level(lv), experience(exp), expToLevelUp(ExpToLevelUp()), gameScene(gameScene)
 {
	 CalculateAttackDamage();
	 CalculateMaxHealth();
 }
void Stats::AddExperience(int exp) {
	experience += exp;
	while (experience >= ExpToLevelUp()) {
		 int diff = experience - ExpToLevelUp();
		experience = 0;
		experience += diff;
		level++;
		CalculateAttackDamage();
		CalculateMaxHealth();
	}
	expToLevelUp = ExpToLevelUp();
}

void Stats::AddExperience(std::shared_ptr<Stats> enemy)
{
	AddExperience(enemy->ExpFromEnemy(level));
}

void Stats::AddHealth(int hp)
{
	currentHealth += hp;
	if (currentHealth > maxHealth) currentHealth = maxHealth;
}

void Stats::IncreaseStats(std::map<std::string, int> stIncr)
{
	for (auto& str : stIncr) {
		if (str.first == "attackDamage") {
			attackDamage += str.second;
		}
	}
}

void Stats::DecreaseStats(std::map<std::string, int> stIncr)
{
	for (auto& str : stIncr) {
		if (str.first == "attackDamage") {
			attackDamage -= str.second;
		}
	}
}
