#include "Selector.h"
#include "Resolution.h"
#include <LevelDataManagement/LevelDataManagement.h>
#include "Mesh.h"


Selector::Selector(float leftDownCornerX, float leftDownCornerY, float width, float height, glm::vec3 color) :
    Proj(glm::ortho(0.0f, Resolution::WIDTH, 0.0f, Resolution::HEIGHT, -1.0f, 1.0f)),
    View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    translation(0.0, 0.0, 0.0), scaleBy(1)
{
    /*
    30.000000,30.000000,1.000000,0.000000,0.000000,
    32.000000,32.000000,1.000000,0.000000,0.000000,
    32.000000,96.000000,1.000000,0.000000,0.000000,
    30.000000,98.000000,1.000000,0.000000,0.000000,
    98.000000,30.000000,1.000000,0.000000,0.000000,
    98.000000,98.000000,1.000000,0.000000,0.000000,
    96.000000,96.000000,1.000000,0.000000,0.000000,
    96.000000,32.000000,1.000000,0.000000,0.000000,
    96.000000,96.000000,1.000000,0.000000,0.000000,
    98.000000,98.000000,1.000000,0.000000,0.000000,
    30.000000,98.000000,1.000000,0.000000,0.000000,
    32.000000,96.000000,1.000000,0.000000,0.000000,
    98.000000,30.000000,1.000000,0.000000,0.000000,
    96.000000,32.000000,1.000000,0.000000,0.000000,
    32.000000,32.000000,1.000000,0.000000,0.000000,
    30.000000,30.000000,1.000000,0.000000,0.000000,



    */
    float verticies[80]
    {
        //left
        -width / 2 - 2,   -height/ 2 - 2,     color.x,color.y,color.z,
        -width / 2,           -height/ 2,                color.x,color.y,color.z,
        -width / 2,           height/ 2,                 color.x,color.y,color.z,
        -width / 2 - 2,   height/ 2 + 2,       color.x,color.y,color.z,
        //right
        width / 2 + 2,   -height/ 2 - 2,      color.x,color.y,color.z,
        width / 2 + 2,   height/ 2 + 2,                color.x,color.y,color.z,
        width / 2,           height/ 2,                color.x,color.y,color.z,
        width / 2,      -height/ 2,       color.x,color.y,color.z,
        //up
        width / 2,   height/ 2,                       color.x,color.y,color.z,
        width / 2 + 2,           height/ 2 + 2,       color.x,color.y,color.z,
        -width / 2 - 2,           height/ 2 + 2,      color.x,color.y,color.z,
        -width / 2,   height/ 2,                      color.x,color.y,color.z,
        //down
        width / 2 + 2,   -height/ 2 - 2,        color.x,color.y,color.z,
        width / 2,       -height/ 2 ,           color.x,color.y,color.z,
        -width / 2,      -height/ 2,            color.x,color.y,color.z,
        -width / 2 - 2,  -height/ 2 - 2,        color.x,color.y,color.z
    };

    unsigned int indicies[] =
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,
    };

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verticies, 80 * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(3);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Selector.shader");
    shader->Bind();
    //PROMENheightTUK ------------------------^
    translation += glm::vec3(leftDownCornerX + width / 2, leftDownCornerY + height/ 2, 0.0);
}

Selector::Selector(std::set<std::pair<unsigned int, unsigned int>> cells, glm::vec3 color) :
    Proj(glm::ortho(0.0f, Resolution::WIDTH, 0.0f, Resolution::HEIGHT, -1.0f, 1.0f)),
    View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
    translation(0.0, 0.0, 0.0), scaleBy(0)
{
    std::vector<std::vector<bool>> selected;
    for (int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        selected.push_back(std::vector<bool>());
        for (int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            selected[i].push_back(false);
        }
    }
    for (const auto& cell : cells) {
        selected[cell.first][cell.second] = true;
    }
    std::vector<float> verts;

    for (int i = 0; i < NUMBER_OF_CELLS_Y; i++) {
        for (int j = 0; j < NUMBER_OF_CELLS_X; j++) {
            if (!selected[i][j]) continue;
            //left
            if (i == 0 || !selected[i-1][j]) {
                verts.push_back((i+1)*LevelDataManagement::cellSize-LevelDataManagement::cellSize/2+-LevelDataManagement::cellSize / 2 - 2);        verts.push_back((j+1)*LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2);      verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2);      verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2);  verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2);verts.push_back( color.x);verts.push_back( color.y);verts.push_back( color.z);
            }
            //right
            if (i == NUMBER_OF_CELLS_X-1 || !selected[i + 1][j]) {
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2);   verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2);   verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2);       verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2);       verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 +  -LevelDataManagement::cellSize / 2);verts.push_back( color.x);verts.push_back( color.y);verts.push_back( color.z);
            }
            //up
            if (j == NUMBER_OF_CELLS_Y-1 || !selected[i][j + 1]) {
                verts.push_back((i+1) * LevelDataManagement::cellSize -LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2);        verts.push_back((j+1)*LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2);   verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2);  verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2);      verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 +  LevelDataManagement::cellSize / 2);verts.push_back( color.x);verts.push_back( color.y);verts.push_back( color.z);
            }
            //down
            if (j == 0 || !selected[i][j - 1]) {
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2 + 2);   verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + LevelDataManagement::cellSize / 2);       verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2);      verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2); verts.push_back(color.x); verts.push_back(color.y); verts.push_back(color.z);
                verts.push_back((i+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2);  verts.push_back((j+1) * LevelDataManagement::cellSize - LevelDataManagement::cellSize/2 + -LevelDataManagement::cellSize / 2 - 2);verts.push_back( color.x);verts.push_back( color.y);verts.push_back( color.z);
            }
        }
    }
    std::vector<unsigned int> ind;
    for (int i = 0; i < verts.size() / 20; i++) {
        ind.push_back(4 * i);
        ind.push_back(1 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(0 + 4 * i);
        ind.push_back(2 + 4 * i);
        ind.push_back(3 + 4 * i);
    }
    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(3);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(ind.data(), ind.size());

    shader = std::make_unique<Shader>("res/shaders/Selector.shader");
    shader->Bind();
    //PROMENI TUK ------------------------^
    translation += glm::vec3(-Mesh::cameraTranslate.x, -Mesh::cameraTranslate.y, 0.0);
}

void Selector::Update()
{

}

void Selector::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    if (scaleBy) model = glm::scale(model, Mesh::scaleCamera);
    mvp = Proj * View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
