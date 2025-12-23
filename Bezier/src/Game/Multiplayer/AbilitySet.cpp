#include "AbilitySet.h"
#include "AbilityIcon.h"
#include <iostream>
AbilitySet::AbilitySet(std::vector<std::string> abilityTextureNames) : currentAbility(0)
{
	maxAbilities = abilityTextureNames.size();
	for (int i = 0; i < maxAbilities; i++) {
		abilities.push_back(std::make_shared<AbilityIcon>(i, abilityTextureNames[i]));
	}
}

void AbilitySet::Update(std::chrono::milliseconds elapsedTime)
{
	for (const auto& ability : abilities) {
		ability->Update(elapsedTime);
	}
}

void AbilitySet::Draw()
{
	for (const auto& ability : abilities) {
		ability->Draw();
	}
}
