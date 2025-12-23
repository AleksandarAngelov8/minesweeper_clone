#include <map>
#include <Game/Item.h>
#include <memory>
#pragma once
class Equipment;
class ItemContainer : public std::enable_shared_from_this<ItemContainer>
{
public:
	std::vector<std::shared_ptr<Item>> items;
	int maxSize;
	ItemContainer();
	ItemContainer(std::vector<std::shared_ptr<Item>> items);
	std::shared_ptr<Item> GetItem(std::string label);
	void EquipWeapon(std::shared_ptr<Equipment> weapon);
	void UnequipWeapon(std::shared_ptr<Equipment> weapon);
	std::shared_ptr<Equipment> equippedWeapon;
	void LoadItemsFromSavedFile();
	bool AddItem(std::shared_ptr<Item> item);
	void RemoveItem(uint32_t index);
	void RemoveItem(std::string itemLabel);
	bool Contains(std::string itemLabel);
};

