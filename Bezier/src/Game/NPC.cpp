#include "NPC.h"
#include <OpenGL/Resolution.h>
#include <Button/ButtonFunctionality.h>
#include <TextureInit/TextureInit.h>
#include "Scene/Edit/Edit.h"
#include "Scene/Game/Game.h"

NPC::NPC(float spawnX, float spawnY, float width, float height, 
    std::string textureName, unsigned int numOfSprites, 
    std::shared_ptr<Scene> scene, std::vector<std::shared_ptr<Quote>> quotes) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
     currentSprite(glm::vec2(0.0, 0.0)), quotes(quotes), scene(scene)
{
    float texture = static_cast<float>(TextureInit::textureMapSToI[textureName]);
    vao = std::make_unique<VertexArray>();

    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        float verticies[16]
        {
            0,   0, 0.0, 0.0, 
             width,    0, 1, 0.0, 
             width,    height ,  1, 1,
            0,   height ,  0.0, 1, 
        };
        vbo = std::make_unique<VertexBuffer>(verticies, 16 * sizeof(float));
        translation += glm::vec3(spawnY, spawnX, 0.0);
    }
    else if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
        float verticies[16]
        {
            spawnY + 0,spawnX +    0, 0.0, 0.0, 
            spawnY + width,spawnX +     0, 1, 0.0, 
            spawnY + width,spawnX +     height ,  1, 1, 
            spawnY + 0,spawnX +    height ,  0.0, 1,
        };
        vbo = std::make_unique<VertexBuffer>(verticies, 16 * sizeof(float));
    }

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Character.shader");
    shader->Bind();
    shader->SetUniform1i("u_TexID", texture);
    
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene))
        dialogbox = std::dynamic_pointer_cast<Game>(scene)->dialogbox;
    else
        dialogbox = nullptr;


}
void NPC::SetUpCellConnections()
{
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        neighbours[UP] = game->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96 + 1);
        neighbours[RIGHT] = game->environment->GetCell(static_cast<unsigned int>(translation.y) / 96 + 1, static_cast<unsigned int>(translation.x) / 96);
        neighbours[DOWN] = game->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96 - 1);
        neighbours[LEFT] = game->environment->GetCell(static_cast<unsigned int>(translation.y) / 96 - 1, static_cast<unsigned int>(translation.x) / 96);
        currentCell = game->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96);
        currentCell->SetInteraction([this]() {
            dialogbox->SetText(quotes[0]->text.c_str());
            dialogbox->shouldBeDrawn = 1;
            //printf("%s", quotes[0]->text.c_str());
            },false);
    }
    else if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
        neighbours[UP] = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96 + 1);
        neighbours[RIGHT] = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / 96 + 1, static_cast<unsigned int>(translation.x) / 96);
        neighbours[DOWN] = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96 - 1);
        neighbours[LEFT] = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / 96 - 1, static_cast<unsigned int>(translation.x) / 96);
        currentCell = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / 96, static_cast<unsigned int>(translation.x) / 96);
    }
}
void NPC::Update()
{
}
void NPC::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    model = glm::translate(model, -Mesh::cameraTranslate);
    model = glm::scale(model, Mesh::scaleCamera);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
