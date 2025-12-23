#pragma once
#include <memory>
#include "map"
#include "string"
class Arena;
class Stats
{
public:
	std::shared_ptr<Arena> gameScene;
	 int maxHealth, attackDamage, baseHealth, baseAttackDamage;
	 int currentHealth;
	 int level, experience, expToLevelUp;
	 void CalculateAttackDamage();
	 void CalculateMaxHealth();
	 int ExpToLevelUp();
	 int Divisor( int level);
	 int ExpFromEnemy( int charLevel);
	 Stats(int mh, int curhp, int ad, int lv, int exp, std::shared_ptr<Arena> gameScene);
	 void AddExperience( int exp);
	void AddExperience(std::shared_ptr<Stats> enemy);
	void AddHealth(int hp);
	void IncreaseStats(std::map<std::string, int> stIncr);
	void DecreaseStats(std::map<std::string, int> stIncr);
};