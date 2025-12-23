#pragma once
#include <vector>
#include <map>
#include "Button/Button.h"
#include "Audio/Audio.h"
#include "OpenGL/Cursor.h"
#include "Button/ButtonFunctionality.h"

class Scene
{
public:
	// if parentScene is unexpectedly nullptr check that parentscene is cast to its correct scene subclass
	int currentActiveButtonIndex;
	std::shared_ptr<Scene> parentScene;
	std::map<int,std::shared_ptr<Button>> buttons;
	std::shared_ptr<Cursor> cursor;
	ButtonLookup bl;
	bool setup = false;
	virtual void SetUp(std::shared_ptr<Scene> parentScene = nullptr)  {
		cursor = std::make_shared<Cursor>();
		for (auto& button : buttons) {
			bl.AddButton(button.second);
		}
	}
	virtual void Update(float xpos, float ypos) {
		if (!cursor->IsClicked && ButtonFunctionality::CheckPressedMouseButton(GLFW_MOUSE_BUTTON_1))
			cursor->SetClicking();
		else if (cursor->IsClicked && !ButtonFunctionality::CheckPressedMouseButton(GLFW_MOUSE_BUTTON_1))
			cursor->SetNotClicking();
		cursor->Update(xpos, ypos);
		if (bl.GetButton(xpos, ypos) != -1) 
			currentActiveButtonIndex = bl.GetButton(xpos, ypos);
	}
	virtual void Draw(bool fromChild = 0) {
		cursor->cursorTexturedRect->Draw();
	}
	virtual void PerformAction(int pressedButtonId) = 0;
	virtual void PerformActionScroll(double ypos) = 0;
	virtual void ReceiveInfo(char* data) = 0;
};

