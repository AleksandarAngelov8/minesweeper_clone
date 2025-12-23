#pragma once
#include "OpenGL/Mesh.h"
#include "Game/Stats.h"
class Bar
{
	glm::vec2 target;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	float scale;
	glm::vec3 color;
	glm::vec3 targetColor;
	bool relativePosition;
public:
	Bar(float spawnX, float spawnY, float width, float height, int *maxVal, int*currVal, glm::vec3 color, glm::vec3 targetColor, bool relativePosition = 0);
	int *maxVal;
	int* currVal;
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;
	void Update(float x = 0, float y = 0);
	void Draw();
};

