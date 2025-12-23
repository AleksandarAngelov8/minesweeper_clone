#pragma once
#include "Scene/Scene.h"
#include "OpenGL/Cursor.h"

class NewMenu : public Scene
{
public:
	std::shared_ptr<Text> text;
	std::shared_ptr<TexturedRect> background;
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
	void ReceiveInfo(char* data) override;
};

