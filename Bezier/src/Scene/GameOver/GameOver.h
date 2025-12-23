#pragma once
#include "Button/ButtonFunctionality.h"
#include "Scene/Scene.h"
class GameOver : public Scene
{
	std::shared_ptr<TexturedRect> background;
	std::shared_ptr<Text> gameOverText;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

