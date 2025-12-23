#pragma once
#include "Scene/Scene.h"
#include "OpenGL/TexturedRect.h"
#include "Game/ItemContainer.h"
class ItemMenu;
class Selector;
class ChestMenu : public Scene, public std::enable_shared_from_this<ChestMenu>
{
	ButtonLookup bl;
	std::shared_ptr<ItemMenu> itemMenu;
	std::shared_ptr<ItemContainer> itemContainerChest;
	std::shared_ptr<TexturedRect> menuBackgroundChest;
	//std::shared_ptr<ItemContainer> itemContainerInventory;
	//std::shared_ptr<TexturedRect> menuBackgroundInventory;
	std::shared_ptr<Selector> selector;
	std::shared_ptr<Game> gameScene;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void MoveItem(std::shared_ptr<ItemContainer> from, std::shared_ptr<ItemContainer> to);
	void SetUpItemsChest();
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

