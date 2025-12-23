#pragma once
#include "OpenGL/Mesh.h"
#include "LevelDataManagement/LevelDataManagement.h"
#include "OpenGL/Environment.h"
#include "glm/gtc/type_ptr.hpp"

class Text : public std::enable_shared_from_this<Text>
{
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
	void InitializeGlyphs();
	float spawnX; 
	float spawnY; 
	float width; 
	float height; 
	glm::vec3 color; 
public:
	std::string text;
	enum align { right, centered, left };
	align alignment;
	glm::vec3 translation;
	Text(float spawnX, float spawnY, float width, float height, const char* text, glm::vec3 color, align alignment);
	void ReinitializeObjects(const char* text);
	std::unique_ptr<Shader> shader;
	void ChangeText(const char* t);

	std::map<char, std::pair<float, float>> glyphPos;
	std::map<char, std::pair<char, char>> previousGlyphPos;

	void Update();
	void Draw();
};

