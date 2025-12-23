#pragma once

#include <vector>
#include <memory>
#include <string>
#include <chrono>

class AbilityIcon;
class AbilitySet
{
public:
	// add a background 
	int currentAbility;
	int maxAbilities;
	std::vector<std::shared_ptr<AbilityIcon>> abilities;
	AbilitySet(std::vector<std::string> abilityTextureNames);
	void Update(std::chrono::milliseconds elapsedTime);
	void Draw();
};

