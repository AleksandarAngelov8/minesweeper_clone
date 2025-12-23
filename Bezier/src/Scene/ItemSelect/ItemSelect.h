#pragma once
#include "Button/ButtonFunctionality.h"
#include "Scene/Scene.h"
#include "OpenGL/DialogBox.h"
#include "Game/Character.h"

class ItemSelect : public Scene, public std::enable_shared_from_this<ItemSelect>
{
	std::shared_ptr<TexturedRect> menuBackground;
	std::shared_ptr<TexturedRect> selectedItem;
	std::shared_ptr<Game> gameScene;
	std::shared_ptr<Text> itemName;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

