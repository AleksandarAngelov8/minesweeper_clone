#include "Bar.h"

Bar::Bar(float spawnX, float spawnY, float width, float height, int* maxVal, int* currVal, glm::vec3 color, glm::vec3 targetColor, bool relativePosition):
    scale(1.0), translation(0.0, 0.0, 0.0), maxVal(maxVal), currVal(currVal), color(color), targetColor(targetColor), relativePosition(relativePosition)
{
    float verticies[8]
    {
        0,   -height/2,        
        width,    -height / 2, 
        width,    height/2,    
        0,   height/2,         
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 8 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Bar.shader");
    shader->Bind();
    //PROMENI TUK ------------------------^

    translation = glm::vec3(spawnX,spawnY + height/2,0.0);

    shader->SetUniformVec3f("u_Color",color);
}



void Bar::Update(float x, float y)
{
    if (relativePosition) {
        translation.x = x;
        translation.y = y;
    }
    scale = float(*currVal) / float(*maxVal);
    shader->Bind();
    shader->SetUniformVec3f("u_Color", scale*color + (1-scale)*targetColor);
}

void Bar::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    if (relativePosition)model = glm::translate(model, -Mesh::cameraTranslate);
    model = glm::scale(model, glm::vec3(scale,1.0,1.0));
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
