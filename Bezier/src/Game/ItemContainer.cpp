#include "ItemContainer.h"
#include "Game/Equipment.h"

ItemContainer::ItemContainer() {}
ItemContainer::ItemContainer(std::vector<std::shared_ptr<Item>> items)
{
	this->items = items;
}
std::shared_ptr<Item> ItemContainer::GetItem(std::string label)
{
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->GetLabel() == label) return items[i];
	}
	std::cout << "Item not found";
	return nullptr;
}
bool ItemContainer::AddItem(std::shared_ptr<Item> item)
{
	if (Contains(item->GetLabel())) {
		GetItem(item->GetLabel())->count++;
		return true;
	}
	if (items.size() >= 16) return false;
	items.push_back(item);
	return true;
}

void ItemContainer::RemoveItem(uint32_t index)
{
	items.erase(items.begin() + index);
}

void ItemContainer::RemoveItem(std::string itemLabel)
{
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->GetLabel() == itemLabel) {
			items.erase(items.begin() + i);
			return;
		}
	}
}

bool ItemContainer::Contains(std::string itemLabel)
{
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->GetLabel(false) == itemLabel) return true;
	}
	return false;
}
void ItemContainer::EquipWeapon(std::shared_ptr<Equipment> weapon)
{
	if (equippedWeapon)
		this->equippedWeapon->equipped = 0;
	equippedWeapon = weapon;
	weapon->equipped = 1;
}

void ItemContainer::UnequipWeapon(std::shared_ptr<Equipment> weapon)
{
	equippedWeapon = nullptr;
	weapon->equipped = 0;
}
