#pragma once
#include <array>
#include <functional>
#include "glm/gtc/type_ptr.hpp"

#include "OpenGL/Text.h"

class Button : public std::enable_shared_from_this<Button>
{
private:
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;

	std::shared_ptr<Text> textObject;
	std::function<void()> action;
public:
	Button(int id, float leftDownCornerX, float leftDownCornerY, float width, float height, glm::vec3 backColor, glm::vec3 textColor, std::string text, bool act);
	void Update();
	void Draw();
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;

	int id;
	std::array<std::shared_ptr<Button>, 4> neighbours;
	std::string type;
	bool isActive;
	float minX, maxX, minY, maxY;
	bool CheckIfHover(float x, float y);
	void PrintPos();
	//neighbours: 0-up,1-right,2-down,3-left
	void SetNeighbourUp(std::shared_ptr<Button> neighbour);
	void SetNeighbourRight(std::shared_ptr<Button> neighbour);
	void SetNeighbourDown(std::shared_ptr<Button> neighbour);
	void SetNeighbourLeft(std::shared_ptr<Button> neighbour);
	std::array<std::shared_ptr<Button>, 4> GetNeighbours();
	void SetAction(std::function<void()> act);
	void Activate();
};

