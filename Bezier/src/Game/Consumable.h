#pragma once
#include "Item.h"
class Consumable : public Item
{
public:
	std::function<void()> onConsume;
	Consumable(std::string label, int count, std::string texture, std::function<void()> action);
	void Consume();
};

