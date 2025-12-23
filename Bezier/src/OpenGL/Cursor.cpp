#include "Cursor.h"
#include "iostream"

Cursor::Cursor() : IsClicked(0)
{
	cursorTexturedRect = std::make_shared<TexturedRect>(0, 0, 78, 78, "cursor", 0, 1, glm::vec2(2,1));
}

void Cursor::Update(float xpos, float ypos)
{
	cursorTexturedRect->translation = glm::vec3(xpos, ypos, 0);
}

void Cursor::SetClicking()
{
	IsClicked = 1;
	cursorTexturedRect->IterateAnimationSprite();
}

void Cursor::SetNotClicking()
{
	IsClicked = 0;
	cursorTexturedRect->IterateAnimationSprite();
}
