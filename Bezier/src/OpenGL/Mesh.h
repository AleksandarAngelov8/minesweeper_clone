#pragma once
#define _USE_MATH_DEFINES
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#include <memory>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Game/Cell.h"
#include <array>
#include "Resolution.h"
class Mesh
{
	float speed;
	glm::vec2 target;
	glm::vec3 scale;
	glm::mat4 mvp;
	std::unique_ptr<VertexArray>  vao;
	std::unique_ptr<IndexBuffer>  ib;
	std::unique_ptr<VertexBuffer> vbo;
public:
	static glm::vec3 scaleCamera;
	static void CenterCamera(float offSetX, float offSetY);
	static glm::vec3 cameraTranslate;
	static glm::mat4 Proj;
	static glm::mat4 View;
	static void MoveCamera(float x, float y);
	static void SetupProj();

	std::array<std::shared_ptr<Cell>, 4> neighbours;
	Mesh(float leftDownCornerX, float leftDownCornerY, float width, float height, std::string textureName);
	Mesh(float leftDownCornerX, float leftDownCornerY, float width, float height, glm::vec3 color);
	Mesh(float leftDownCornerX, float leftDownCornerY, float width, float height, float texture, unsigned int numOfSprites);
	unsigned int rotation;
	bool movingLeft;
	bool movingRight;
	bool movingUp;
	bool movingDown;

	void Update();
	void Draw();
	void ScaleX(float x);
	void ScaleY(float y);
	void SetNeighboursMoving(std::shared_ptr<Cell> up, std::shared_ptr<Cell> right, std::shared_ptr<Cell> down, std::shared_ptr<Cell> left);
	std::unique_ptr<Shader> shader;
	glm::vec3 translation;
	float X();
	float Y();
	void X(float x);
	void Y(float y);
	void MoveUp();
	void MoveRight();
	void MoveDown();
	void MoveLeft();
	void StartMoveAnimationUp();
	void StartMoveAnimationDown();
	void StartMoveAnimationRight();
	void StartMoveAnimationLeft();
	bool UpCollision();
	bool RightCollision();
	bool DownCollision();
	bool LeftCollision();
};

