#include "Button/Button.h"
#include <TextureInit/TextureInit.h>
//BUTTON TYPES: 0 = buttonPlay; 1 = buttonEdit; 2 = buttonExit
Button::Button(int id, float leftDownCornerX, float leftDownCornerY, float width, float height, glm::vec3 backColor, glm::vec3 textColor, std::string text, bool act) : type(text), isActive(act), id(id), action([]() {}),
	scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0)
{
    minX = leftDownCornerX + -width / 2;
    maxX = leftDownCornerX + width / 2;
    minY = leftDownCornerY + -height / 2;
    maxY = leftDownCornerY + height / 2;
    float verticies[40]
    {
        -width / 2,   -height / 2,                     backColor.x,backColor.y,backColor.z,
        width/2,    -height / 2,                backColor.x,backColor.y,backColor.z,
        width/2,    height/2,           backColor.x,backColor.y,backColor.z,
        -width / 2,   height/2,                backColor.x,backColor.y,backColor.z,
        
        -width/2 + height / 10,   -height/2 + height / 10,                    backColor.x + backColor.x/2,backColor.y + backColor.y / 2,backColor.z + backColor.z / 2,
        width/2 - height / 10,    -height/2 + height / 10,           backColor.x + backColor.x / 2,backColor.y + backColor.y / 2,backColor.z + backColor.z / 2,
        width/2 - height / 10,    height/2 - height / 10,  backColor.x + backColor.x / 2,backColor.y + backColor.y / 2,backColor.z + backColor.z / 2,
        -width/2 + height / 10,   height/2 - height / 10,           backColor.x + backColor.x / 2,backColor.y + backColor.y / 2,backColor.z + backColor.z / 2
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 40 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(3);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Button.shader");
    shader->Bind();
    shader->SetUniform1f("u_Oppacity", 1);
    //PROMENI TUK ------------------------^
    translation += glm::vec3(leftDownCornerX, leftDownCornerY, 0.0);
    float fontSizeMultiplier = Resolution::WIDTH / 1920.0f;
    textObject = std::make_shared<Text>(leftDownCornerX,leftDownCornerY,fontSizeMultiplier * 32.0, fontSizeMultiplier * 32.0,text.c_str(), textColor, Text::centered);
}
void Button::Update()
{
}
void Button::Draw()
{
    Renderer renderer;
    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    //model = glm::translate(model, -Mesh::cameraTranslate);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    //model = glm::scale(model, scaleCamera);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
    textObject->Draw();
}
void Button::SetNeighbourUp(std::shared_ptr<Button> neighbour)
{
	neighbours[0] = neighbour;
	if (neighbour->GetNeighbours()[2] != shared_from_this())
		neighbour->SetNeighbourDown(shared_from_this());
}
void Button::SetNeighbourRight(std::shared_ptr<Button> neighbour)
{
	neighbours[1] = neighbour;
	if (neighbour->GetNeighbours()[3] != shared_from_this())
		neighbour->SetNeighbourLeft(shared_from_this());
}
void Button::SetNeighbourDown(std::shared_ptr<Button> neighbour)
{
	neighbours[2] = neighbour;
	if (neighbour->GetNeighbours()[0] != shared_from_this())
		neighbour->SetNeighbourUp(shared_from_this());
}
void Button::SetNeighbourLeft(std::shared_ptr<Button> neighbour)
{
	neighbours[3] = neighbour;
	if (neighbour->GetNeighbours()[1] != shared_from_this())
		neighbour->SetNeighbourRight(shared_from_this());
}
std::array<std::shared_ptr<Button>, 4> Button::GetNeighbours()
{
	return neighbours;
}
void Button::SetAction(std::function<void()> act)
{
	action = act;
}
void Button::Activate()
{
    if (action == nullptr) printf("Empty action for button with id %i", id);
	action();
}
bool Button::CheckIfHover(float x, float y)
{
	return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

void Button::PrintPos()
{
	printf("%f - %f | %f - %f", minX, maxX, minY, maxY);
}
