#pragma once
#include <string>
#include <memory>
#include "OpenGL/TexturedRect.h"
#include <OpenGL/Text.h>

class TexturedRect;
class ItemContainer;
class Item : public std::enable_shared_from_this<Item>
{
	std::string label;
public:
	virtual ~Item() = default;
	Item(std::string label, int count, std::string texture, bool stackable);
	std::string GetLabel(bool full = 1);
	// for nonstackable
	static std::map<std::string, int> uniqueIndices;
	bool stackable;
	int count;
	std::string texture;
	mutable std::shared_ptr<TexturedRect> inventoryTexture;
	mutable std::shared_ptr<Text> countText;
	std::shared_ptr<TexturedRect> floorTexture;

	void BindItemToCell(std::shared_ptr<Environment> environment, std::shared_ptr<Game> gameScene, std::shared_ptr<Cell> cell);
	virtual void SetTextureInInventory(int x, int y, bool right = 0) const;
};

