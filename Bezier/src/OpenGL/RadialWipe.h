#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
#include <memory>

class RadialWipe
{
public:
	glm::vec3 color;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<VertexBufferLayout> layout;
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;
	glm::mat4 mvp;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	float radius;
	float centerX, centerY;
	float angle; // in degrees
	RadialWipe(float cx, float cy, float r);
	void SetAngle(float progress = 0.5f);
	void Update();
	void Draw();
};

