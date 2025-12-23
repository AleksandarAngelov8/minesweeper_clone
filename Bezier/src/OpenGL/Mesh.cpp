#include "Mesh.h"
#include "Button/ButtonFunctionality.h"
#include <iostream>
#include <TextureInit/TextureInit.h>
#include "LevelDataManagement/LevelDataManagement.h"

// NAPRAVI CHARACTER DA NE E ZAVISIM OT MESH
// OPRAVI NEIGHBOURS
void Mesh::CenterCamera(float offSetX, float offSetY)
{
    //cameraTranslate = glm::vec3(-Resolution::WIDTH/2 + offSetX, -Resolution::HEIGHT/2 + offSetY, 0.0);
    scaleCamera = glm::vec3(1.0, 1.0, 1.0);
}

void Mesh::MoveCamera(float x, float y)
{
    cameraTranslate = glm::vec3(cameraTranslate.x+x, cameraTranslate.y+y,0);
}

void Mesh::SetupProj()
{
    Proj = glm::ortho(0.0f, Resolution::WIDTH, 0.0f, Resolution::HEIGHT, -1.0f, 1.0f);
    View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

Mesh::Mesh(float leftDownCornerX, float leftDownCornerY,
    float width, float height, std::string textureName) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
    movingUp(false), movingDown(false), movingRight(false), movingLeft(false),
    speed(2.0)
{
    float texture = static_cast<float>(TextureInit::textureMapSToI[textureName]);
    float verticies[20]
    {
        -width / 2,   -height / 2, 0.0, 0.0, texture,
        width / 2,    -height / 2, 1.0, 0.0, texture,
        width / 2,    height / 2,  1.0, 1.0, texture,
        -width / 2,   height / 2,  0.0, 1.0, texture
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 20 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    layout1.Push<float>(1);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Game.shader");
    shader->Bind();
    shader->SetUniformVec1i("u_Texture", 25);
    //PROMENI TUK ------------------------^
    shader->SetUniform1f("u_TexID", static_cast<float>(texture));
    translation += glm::vec3(leftDownCornerX + width / 2, leftDownCornerY + height / 2, 0.0);

    neighbours = { nullptr ,nullptr ,nullptr ,nullptr };
    
}
Mesh::Mesh(float leftDownCornerX, float leftDownCornerY,
    float width, float height, glm::vec3 color) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
    movingUp(false), movingDown(false), movingRight(false), movingLeft(false),
    speed(2.0)
{
    float verticies[20]
    {
        0,   0, color.x,color.y,color.z,
        width,    0, color.x,color.y,color.z,
        width,    height,  color.x,color.y,color.z,
        0,   height,  color.x,color.y,color.z
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 20 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(3);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Selector.shader");
    shader->Bind();
    shader->SetUniform1f("u_Oppacity", 0.3f);
    //PROMENI TUK ------------------------^
    translation += glm::vec3(leftDownCornerX + width / 2, leftDownCornerY + height / 2, 0.0);

    neighbours = { nullptr ,nullptr ,nullptr ,nullptr };

}
Mesh::Mesh(float leftDownCornerX, float leftDownCornerY, 
    float width, float height, 
    float texture, unsigned int numOfSprites) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
    movingUp(false), movingDown(false), movingRight(false), movingLeft(false),
    speed(2.0)
{
    float verticies[20]
    {
        -width / 2,   -height / 2, 0.0, 0.0, texture,
        width / 2,    -height / 2, 0.25, 0.0, texture,
        width / 2,    height / 2,  0.25, 1.0, texture,
        -width / 2,   height / 2,  0.0, 1.0, texture
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 20 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    layout1.Push<float>(1);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Game.shader");
    shader->Bind();
    shader->SetUniformVec1i("u_Texture", 25);
    //PROMENI TUK ------------------------^
    shader->SetUniform1f("u_TexID", texture);
    translation += glm::vec3(leftDownCornerX + width / 2, leftDownCornerY + height / 2, 0.0);

    neighbours = { nullptr ,nullptr ,nullptr ,nullptr };
}
void Mesh::Update()
{
    movingUp = movingUp && (neighbours[UP] != nullptr && neighbours[UP]->walkable);
    movingDown = movingDown && (neighbours[DOWN] != nullptr && neighbours[DOWN]->walkable);
    movingRight = movingRight && (neighbours[RIGHT] != nullptr && neighbours[RIGHT]->walkable);
    movingLeft = movingLeft && (neighbours[LEFT] != nullptr && neighbours[LEFT]->walkable);

    movingUp = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_UP);
    movingDown = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_DOWN);
    movingRight = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_RIGHT);
    movingLeft = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_LEFT);
    //printf("%f.2,%f.2|%f.2,%f.2\n", translation.x, translation.y, Mesh::cameraTranslate.x, Mesh::cameraTranslate.y);
    
}
void Mesh::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    model = glm::translate(model, translation); 
    model = glm::translate(model, -cameraTranslate);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    //model = glm::scale(model, scaleCamera);
    mvp = Proj * View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao,*ib,*shader);
}
void Mesh::ScaleX(float x)
{
    scale = glm::vec3(x, scale.y, scale.z);
}
void Mesh::ScaleY(float y)
{
    scale = glm::vec3(scale.x, y, scale.z);
}
void Mesh::SetNeighboursMoving(std::shared_ptr<Cell> up, std::shared_ptr<Cell> right, std::shared_ptr<Cell> down, std::shared_ptr<Cell> left)
{
    neighbours[UP] = up;
    neighbours[RIGHT] = right;
    neighbours[DOWN] = down;
    neighbours[LEFT] = left;
}
float Mesh::X()
{
    return translation.x;
}
float Mesh::Y()
{
    return translation.y;
}
void Mesh::X(float x) {
    translation.x = x;
}
void Mesh::Y(float y) {
    translation.y = y;
}
void Mesh::MoveUp()
{
    //if ((!neighbours[UP] || (!neighbours[UP]->walkable) && 
    //    translation.y >= (neighbours[UP]->position.second)) * ButtonFunctionality::cellSize
    //    ) return;
    translation.y += speed;
    Mesh::cameraTranslate.y += speed;
    movingUp = true;
    rotation = UP;
}
void Mesh::MoveRight()
{
    //if ((!neighbours[RIGHT] || (!neighbours[RIGHT]->walkable) && 
    //    translation.x >= (neighbours[RIGHT]->position.first)) * ButtonFunctionality::cellSize
    //    ) return;
    translation.x += speed;
    Mesh::cameraTranslate.x += speed;
    movingRight = true;
    rotation = RIGHT;
}
void Mesh::MoveDown()   
{
    //if ((!neighbours[DOWN] || (!neighbours[DOWN]->walkable) &&
    //    translation.y <= (neighbours[DOWN]->position.second)) * ButtonFunctionality::cellSize
    //    ) return; 
    translation.y -= speed;
    Mesh::cameraTranslate.y -= speed;
    movingDown = true;
    rotation = DOWN;
}
void Mesh::MoveLeft()
{
    //if ((!neighbours[LEFT] || (!neighbours[LEFT]->walkable) &&
    //    translation.x <= (neighbours[LEFT]->position.first)) * ButtonFunctionality::cellSize
    //    ) return;
    translation.x -= speed;
    Mesh::cameraTranslate.x -= speed;
    movingLeft = true;
    rotation = LEFT;
}
void Mesh::StartMoveAnimationUp()
{
    if (!(movingRight || movingLeft || movingDown)) {
        target = glm::vec2(translation.x, translation.y + 2 + ((int)(ButtonFunctionality::cellSize) - (int)(translation.y+2) % (int)(ButtonFunctionality::cellSize)));
        movingUp = true;
        rotation = UP;
    }
}
void Mesh::StartMoveAnimationDown()
{
    if (!(movingRight || movingLeft || movingUp)){
        if (translation.y <= (int)(ButtonFunctionality::cellSize)*3/2) {
            target = glm::vec2(translation.x, (int)(ButtonFunctionality::cellSize) / 2);
        }
        else
        {
            target = glm::vec2(translation.x, translation.y - 2 - ((int)(translation.y - 2) - (int)(ButtonFunctionality::cellSize)) % (int)(ButtonFunctionality::cellSize));
        }
        rotation = DOWN;
        movingDown = true;
    }
}
void Mesh::StartMoveAnimationRight()
{
    if (!(movingUp || movingLeft || movingDown)) {
        target = glm::vec2(translation.x + 2 + ((int)(ButtonFunctionality::cellSize) - (int)(translation.x + (int)(ButtonFunctionality::cellSize)/2+2) % (int)(ButtonFunctionality::cellSize)), translation.y);
        movingRight = true;
        rotation = RIGHT;
    }
}
void Mesh::StartMoveAnimationLeft()
{
    if (!(movingRight || movingUp || movingDown)) {
        if (translation.x <= (int)(ButtonFunctionality::cellSize)*3/2) {
            target = glm::vec2((int)(ButtonFunctionality::cellSize)/2, translation.y);
        }
        else {
            target = glm::vec2(translation.x - 2 - ((int)(translation.x - (int)(ButtonFunctionality::cellSize)/2-2) - (int)(ButtonFunctionality::cellSize)) % (int)(ButtonFunctionality::cellSize), translation.y);
        }
        rotation = LEFT;
        movingLeft = true;
    }
}
bool Mesh::UpCollision()
{
    return false;
}
bool Mesh::RightCollision()
{
    return false;
}
bool Mesh::DownCollision()
{
    return false;
}
bool Mesh::LeftCollision()
{
    return false;
}
glm::mat4 Mesh::Proj = glm::ortho(0.0f, Resolution::WIDTH, 0.0f, Resolution::HEIGHT, -1.0f, 1.0f);
glm::mat4 Mesh::View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
glm::vec3 Mesh::cameraTranslate = glm::vec3(-Resolution::WIDTH, -Resolution::HEIGHT, 0.0);
glm::vec3 Mesh::scaleCamera = glm::vec3(1.0,1.0,1.0);