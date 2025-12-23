#include "FloatingIndicator.h"

FloatingIndicator::FloatingIndicator(float spawnX, float spawnY, float width, float height, std::string info, glm::vec3 color)
	:color(color), oppacity(1.0), shouldBeDeleted(false)
{
	text = std::make_unique<Text>(spawnX, spawnY, width, height, info.c_str(), color, Text::centered);
	textBack = std::make_unique<Text>(spawnX-2,spawnY-2,width+4,height+4, info.c_str(), glm::vec3(0.0,0.0,0.0), Text::centered);
}

void FloatingIndicator::Update()
{
	oppacity -= 0.01;
	if (oppacity <= 0) {
		shouldBeDeleted = true;
	}
	text->shader->Bind();
	text->shader->SetUniform1f("u_Oppacity", oppacity);
	textBack->shader->SetUniform1f("u_Oppacity",oppacity);
	text->translation = glm::vec3(text->translation.x, text->translation.y + oppacity, text->translation.z);
	textBack->translation = glm::vec3(text->translation.x, text->translation.y + oppacity, text->translation.z);
}
void FloatingIndicator::Draw()
{
	if (!shouldBeDeleted) {
		textBack->Draw();
		text->Draw();
	}
}
