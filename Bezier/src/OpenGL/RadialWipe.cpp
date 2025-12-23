#include "RadialWipe.h"
#include "Mesh.h"
#include "glm/gtc/type_ptr.hpp"
#include <numbers>
#include <iostream>
RadialWipe::RadialWipe(float cx, float cy, float r)
{
	// use the color instead hardcoded values
    color = glm::vec3(0.1, 0.1, 0.1);
    angle = 0.0f;
    radius = r;
	centerX = cx;
	centerY = cy;

    vertices = {
		0, 0,                   color.x, color.y, color.z,                      // 0
        0, 0 + radius,          color.x, color.y, color.z,             // 1
        0 - radius, 0 + radius, color.x, color.y, color.z,    // 2
        0 - radius, 0 - radius, color.x, color.y, color.z,    // 3
        0 + radius, 0 - radius, color.x, color.y, color.z,    // 4
        0 + radius, 0 + radius, color.x, color.y, color.z,    // 5
        0 + radius, 0 + radius, color.x, color.y, color.z,    // 6 designated for the point on the square edge
    };

    vao = std::make_unique<VertexArray>();
    layout = std::make_unique<VertexBufferLayout>();
    
    layout->Push<float>(2);
    layout->Push<float>(3);
    
    shader = std::make_unique<Shader>("res/shaders/RadialWipe.shader");
    shader->Bind();
    shader->SetUniform1f("u_Oppacity", 0.7);
    translation += glm::vec3(centerX, centerY,0.0f);
    SetAngle(0.0f);
}

void RadialWipe::SetAngle(float progress)
{
    assert(progress >= 0.0f && progress <= 1.0f);
    angle = progress * 360.0f;

    float pi = std::numbers::pi;
    float radians = angle * (pi / 180.0f);

    indices.clear();

    if (angle >= 0.0f && angle < 45.0f){
        vertices[30] = -(tan(radians) * radius);
        vertices[31] = radius;
        indices = { 0,1,6 };
    }
    else if (angle >= 45.0f && angle < 135.0f) {
        vertices[30] = -radius;
        vertices[31] = -(tan(radians - pi/2) * radius);
        indices = { 
            0,1,2,
            0,2,6,
        };
    }
    else if (angle >= 135.0f && angle < 225.0f) {
        vertices[30] = (tan(radians - pi) * radius);
        vertices[31] = -radius;
        indices = {
            0,1,2,
            0,2,3,
            0,3,6,
        };
    }
    else if (angle >= 225.0f && angle < 315.0f){
		vertices[30] = radius;
        vertices[31] = (tan(radians - 3*pi/2) * radius);;
        indices = {
            0,1,2,
            0,2,3,
            0,3,4,
            0,4,6,
        };
    }
    else {
        vertices[30] = -(tan(radians) * radius);
        vertices[31] = radius;
        indices = {
            0,1,2,
            0,2,3,
            0,3,4,
            0,4,5,
            0,5,6,
        };
    }

    vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    vbo->Bind();
    vao->AddBuffer(*vbo, *layout);
    ib = std::make_unique<IndexBuffer>(indices.data(), (unsigned int)indices.size());

}



void RadialWipe::Update()
{

}

void RadialWipe::Draw()
{
    Renderer renderer;
    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    //model = glm::translate(model, -Mesh::cameraTranslate);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    //model = glm::scale(model, scaleCamera);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
}
