#pragma once
#include "OpenGL/TexturedRect.h"
class Cursor
{
public:
	bool IsClicked;
	Cursor();
	std::shared_ptr<TexturedRect> cursorTexturedRect;
	void Update(float xpos,float ypos);
	void SetClicking();
	void SetNotClicking();
};

