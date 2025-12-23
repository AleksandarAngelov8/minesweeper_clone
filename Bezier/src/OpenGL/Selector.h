#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"
#include <memory>
#include <set>

#pragma once
class Selector
{
private:
	glm::mat4 Proj;
	glm::mat4 View;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
public:
	bool scaleBy;
	Selector(float leftDownCornerX, float leftDownCornerY, float width, float height, glm::vec3 color);
	Selector(std::set<std::pair<unsigned int, unsigned int>> cells, glm::vec3 color);
	void Update();
	void Draw();
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;
};

