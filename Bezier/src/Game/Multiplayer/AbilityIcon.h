#pragma once
#include "OpenGL/TexturedRect.h"
#include "AbilitySet.h"
#include "OpenGL/RadialWipe.h"

#include <memory>
#include <chrono>

class AbilityIcon
{
public:
	// in milliseconds
	bool onCooldown;
	std::chrono::milliseconds cooldownduration;
	std::chrono::milliseconds elapsedTimeSinceLastUse;
	
	bool ActivateAbility();
	std::unique_ptr<RadialWipe> radialWipe;
	std::unique_ptr<TexturedRect> iconSquare;
	std::shared_ptr<AbilitySet> abilitySet;
	AbilityIcon(int iconId, std::string abilityTextureName);
	void Update(std::chrono::milliseconds elapsedTime);
	void Draw();
};

