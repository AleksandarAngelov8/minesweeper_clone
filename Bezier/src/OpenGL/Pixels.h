#pragma once
#define _USE_MATH_DEFINES
#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"
class Pixels
{
	glm::mat4 Proj;
	glm::mat4 View;
	glm::mat4 mvp;
	std::vector<std::unique_ptr<VertexArray>> vao;
	std::unique_ptr<IndexBuffer> ib;
	std::vector<std::unique_ptr<VertexBuffer>> vbo;
public:
	Pixels(std::vector<std::pair<float, float>> pixels, int color);
	void Draw();
	std::vector<std::unique_ptr<Shader>> shader;
	std::vector<glm::vec3> translation;
	float X(unsigned int index);
	float Y(unsigned int index);
	void X(unsigned int index, float x);
	void Y(unsigned int index, float y);
	void TranslatePixels(std::vector<std::pair<float, float>> pixels);
};

