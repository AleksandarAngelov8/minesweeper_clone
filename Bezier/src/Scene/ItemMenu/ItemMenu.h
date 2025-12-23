#pragma once
#include "Scene/Scene.h"
#include "OpenGL/TexturedRect.h"
#include "Game/ItemContainer.h"

class Selector;
class ItemMenu : public Scene, public std::enable_shared_from_this<ItemMenu>
{
public:
	std::shared_ptr<ItemContainer> itemContainer;
	std::shared_ptr<TexturedRect> menuBackground;
	std::shared_ptr<Selector> selector;
	std::shared_ptr<Game> gameScene;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void SetUpItems();
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

