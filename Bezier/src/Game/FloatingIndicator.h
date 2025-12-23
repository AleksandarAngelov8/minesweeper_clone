#pragma once
#include "OpenGL/Text.h"

class FloatingIndicator
{
	float oppacity;
	glm::vec3 color;
	std::unique_ptr<Text> text;
	std::unique_ptr<Text> textBack;
public:
	bool shouldBeDeleted;
	FloatingIndicator(float spawnX, float spawnY, float width, float height, std::string info, glm::vec3 color);
	void Update();
	void Draw();
};

