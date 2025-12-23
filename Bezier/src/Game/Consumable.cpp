#include "Consumable.h"
Consumable::Consumable(std::string label, int count, std::string texture, std::function<void()> action) 
	: Item(label, count, texture, 1), onConsume(action) {

}

void Consumable::Consume()
{
	onConsume();
	count--;
}
