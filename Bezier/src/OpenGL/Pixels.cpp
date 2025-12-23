#include "Pixels.h"
#include "Resolution.h"

Pixels::Pixels(std::vector<std::pair<float, float>> pixels, int color) :
    Proj(glm::ortho(0.0f, Resolution::WIDTH, 0.0f, Resolution::HEIGHT, -1.0f, 1.0f)),
    View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    layout.Push<float>(1);
    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));
    for (size_t i = 0; i < pixels.size(); i++)
    {
        float verticies[20]
        {
            0, 0, 0.0, 0.0, 0,
            0 + 5, 0, 1.0, 0.0, 0,
            0 + 5, 0 + 5, 1.0, 1.0, 0,
            0, 0 + 5, 0.0, 1.0, 0
        };

       

        vao.push_back(std::make_unique<VertexArray>());
        vbo.push_back(std::make_unique<VertexBuffer>(verticies, 20 * sizeof(float)));
        
        vbo[i]->Bind();
        vao[i]->AddBuffer(*vbo[i], layout);

        shader.push_back(std::make_unique<Shader>("res/shaders/Game.shader"));
        shader[i]->Bind();
        shader[i]->SetUniformVec1i("u_Texture", 100);
        //PROMENI TUK ------------------------^
        shader[i]->SetUniform1f("u_TexID", color);
        translation.push_back(glm::vec3(pixels[i].first, pixels[i].second, 0.0));
    }
}
void Pixels::Draw()
{
    Renderer renderer;
    
    for (size_t i = 0; i < shader.size(); i++)
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, translation[i]);
        mvp = Proj * View * model;

        shader[i]->Bind();
        shader[i]->SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(*vao[i], *ib, *shader[i]);
    }
}
float Pixels::X(unsigned int index)
{
    return translation[index].x - 20;
}
float Pixels::Y(unsigned int index)
{
    return translation[index].y - 20;
}
void Pixels::X(unsigned int index, float x) {
    translation[index].x = x;
}
void Pixels::Y(unsigned int index, float y) {
    translation[index].y = y;
}

void Pixels::TranslatePixels(std::vector<std::pair<float, float>> newTranslations)
{
    for (size_t i = 0; i < newTranslations.size(); i++)
    {
        translation[i].x = newTranslations[i].first;
        translation[i].y = newTranslations[i].second;
    }
}
