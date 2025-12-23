#pragma once
#include <memory>
#include "glm/gtc/type_ptr.hpp"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

class TexturedRect
{
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	glm::vec2 numOfSprites;
	bool relativePosition;
public:
	TexturedRect(float leftDownCornerX, float leftDownCornerY, float width, float height, std::string textureName, 
		bool relativePosition = 0, bool shouldBeDrawn = 1, glm::vec2 numOfSprites = glm::vec2(1.0, 1.0));
	void IterateAnimationSprite();
	glm::vec2 currentSprite;
	void Update();
	void Draw();
	bool shouldBeDrawn;
	std::string texture;
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;
};

