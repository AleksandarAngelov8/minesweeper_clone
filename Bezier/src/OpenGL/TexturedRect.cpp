#include "TexturedRect.h"
#include <TextureInit/TextureInit.h>
#include "Mesh.h"
#include <iostream>

TexturedRect::TexturedRect(float leftDownCornerX, float leftDownCornerY, float width, float height, std::string textureName, bool relativePosition, bool shouldBeDrawn, glm::vec2 numOfSprites) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), numOfSprites(numOfSprites), relativePosition(relativePosition), texture(textureName), currentSprite(glm::vec2(0.0, 0.0))
{
    unsigned int texture = TextureInit::textureMapSToI[textureName];
    float verticies[16]
    {
        -width / 2,   -height / 2, 0.0, 0.0,
        width / 2,    -height / 2, 1.0 / numOfSprites.x, 0.0,
        width / 2,    height / 2,  1.0 / numOfSprites.x, 1.0 / numOfSprites.y,
        -width / 2,   height / 2,  0.0, 1.0 / numOfSprites.y,
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 16 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/TexturedRect.shader");
    shader->Bind();
    shader->SetUniform1i("u_TexID", texture);
    translation += glm::vec3(leftDownCornerX + width / 2, leftDownCornerY + height / 2, 0.0);
}

void TexturedRect::IterateAnimationSprite()
{
    if (currentSprite.x >= 1 - 1.0 / numOfSprites.x) currentSprite = glm::vec2(0.0, currentSprite.y);
    else currentSprite = glm::vec2(currentSprite.x + 1.0 / numOfSprites.x, currentSprite.y);
    shader->SetUniformVec2f("u_texCoordOffSet", currentSprite);
}

void TexturedRect::Update()
{
    int tmpResX = 1920, tmpResY = 1080;

    if (translation.x > tmpResX) translation = glm::vec3(tmpResX, translation.y, 0);
    if (translation.y > tmpResY) translation = glm::vec3(translation.x, tmpResY, 0);
    if (translation.x < 0) translation = glm::vec3(0, translation.y, 0);
    if (translation.y < 0) translation = glm::vec3(translation.x, 0, 0);
}

void TexturedRect::Draw()
{
    Renderer renderer;
    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    if (relativePosition) model = glm::translate(model, -Mesh::cameraTranslate);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    //model = glm::scale(model, scaleCamera);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
