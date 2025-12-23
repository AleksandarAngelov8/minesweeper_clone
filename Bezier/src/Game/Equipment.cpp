#include "Equipment.h"

Equipment::Equipment(std::string label, std::string texture, std::map<std::string, int> statIncrease) : statIncrease(statIncrease), Item(label, 1, texture, 0), equipped(0) {
}
void Equipment::SetTextureInInventory(int x, int y, bool right) const
{
	Item::SetTextureInInventory(x, y, right);
	equippedSymbol = std::make_shared<TexturedRect>((2 + right * 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + x * 128, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - y * 128 - 50, 96, 96, "equipped");
}
