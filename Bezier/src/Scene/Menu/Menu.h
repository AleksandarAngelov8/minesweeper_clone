#pragma once
#include "Button/ButtonFunctionality.h"
#include "Scene/Scene.h"
#include "OpenGL/DialogBox.h"
#include "Game/Character.h"
class Menu : public Scene
{
public:
	std::shared_ptr<DialogBox> dialogbox;
	std::shared_ptr<Text> info;
	std::shared_ptr<Arena> arenaScene;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0);
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

