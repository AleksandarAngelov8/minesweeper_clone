#include "Game/Item.h"
#pragma once
class Equipment : public Item
{
public:
	Equipment(std::string label, std::string texture, std::map<std::string, int> statIncrease);
	void SetTextureInInventory(int x, int y, bool right) const override;
	mutable std::shared_ptr<TexturedRect> equippedSymbol;
	bool equipped;
	std::map<std::string, int> statIncrease;
};

