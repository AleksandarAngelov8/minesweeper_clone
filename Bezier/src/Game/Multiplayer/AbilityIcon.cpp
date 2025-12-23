#include "AbilityIcon.h"
#include <iostream>

bool AbilityIcon::ActivateAbility()
{
	if (onCooldown) return false;
	onCooldown = true;
	elapsedTimeSinceLastUse = std::chrono::milliseconds(0);
	return true;
}

AbilityIcon::AbilityIcon(int iconId, std::string abilityTextureName) : cooldownduration(1000), elapsedTimeSinceLastUse(0), onCooldown(false)
{
	iconSquare = std::make_unique<TexturedRect>(
		400 + iconId * 96, 40, 64, 64 , abilityTextureName
	);
	radialWipe = std::make_unique<RadialWipe>(
		400 + iconId * 96 + 64/2, 40 + 64 / 2, 32
	);
}

void AbilityIcon::Update(std::chrono::milliseconds elapsedTime)
{
	iconSquare->Update();
	if (onCooldown) {
		elapsedTimeSinceLastUse += elapsedTime;
		if (elapsedTimeSinceLastUse >= cooldownduration) {
			onCooldown = false;
		}
		else {
			float progress =
				std::chrono::duration<float>(elapsedTimeSinceLastUse).count() /
				std::chrono::duration<float>(cooldownduration).count();
			radialWipe->SetAngle(1-progress);
		}
	}
	radialWipe->Update();
}

void AbilityIcon::Draw()
{
	iconSquare->Draw();
	radialWipe->Draw();
}
