#pragma once
#include <memory>

class TexturedRect;
class AttackWarning
{
public:
	AttackWarning(std::shared_ptr<TexturedRect> textureObj, int wI);
	std::shared_ptr<TexturedRect> textureObject;
	int warningIndex;
};

