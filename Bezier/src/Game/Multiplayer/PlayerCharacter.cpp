#include "PlayerCharacter.h"
#include <OpenGL/Resolution.h>
#include <Button/ButtonFunctionality.h>
#include <TextureInit/TextureInit.h>
#include "Scene/MultiplayerScenes/Arena.h"

#define cellSize ButtonFunctionality::cellSize

PlayerCharacter::PlayerCharacter(float spawnX, float spawnY, float width, float height,
    std::string textureName, glm::vec2 numOfSprites, std::shared_ptr<Scene> scene,
    float maxhp, float currhp, float ad, unsigned int lvl, int exp) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), 
    speed(4), currentSprite(glm::vec2(0.0, 0.0)), numOfSprites(numOfSprites),
    moving({false,false,false,false}),
    animationSwapTime(std::chrono::milliseconds(0)), movementLimits(std::make_pair(0,Resolution::HEIGHT/4))
{
	rotation = 0;
    scene = scene;
    prevTranslation = glm::vec3(0.0, 0.0, 0.0);
    differenceInPosition = glm::vec3(0.0, 0.0, 0.0);
    float texture = static_cast<float>(TextureInit::textureMapSToI[textureName]);
    float verticies[16]
    {
        -width / 2 + 3,   -8,           0.0, 0.0,
        width / 2 + 3,    -8,           1.0 / numOfSprites.x, 0.0,
        width / 2 + 3,    height - 8,   1.0 / numOfSprites.x, 1.0 / numOfSprites.y,
        -width / 2 + 3,   height - 8,   0.0, 1.0 / numOfSprites.y,
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

    shader = std::make_unique<Shader>("res/shaders/Character.shader");
    shader->Bind();
    //PROMENI TUK ------------------------^
    shader->SetUniform1i("u_TexID", (texture));
    translation += glm::vec3(spawnX + cellSize / 2, spawnY + cellSize / 2, 0.0);
    //Mesh::cameraTranslate = glm::vec3(spawnX, spawnY, 0.0);
    //Mesh::CenterCamera(spawnX, spawnY);

    if (std::shared_ptr<Arena> arena = std::dynamic_pointer_cast<Arena>(scene)) {
        stats = std::make_shared<Stats>(maxhp, currhp, ad, lvl, exp, arena);
    }

    abilitySet = std::make_unique<AbilitySet>(std::vector<std::string>{"attack_1"});

}
std::vector<std::shared_ptr<Missile>> PlayerCharacter::GetMissiles()
{
    return missiles;
}
void PlayerCharacter::CreateMissile(glm::vec2 mousePos, float speed)
{
    if (abilitySet->abilities[abilitySet->currentAbility]->ActivateAbility()) {
        missilesTrigger.push_back({ mousePos, speed });
    }
}
int PlayerCharacter::GetRotation()
{
    return rotation;
}
bool PlayerCharacter::GetMoving(int i)
{
    return moving[i];
}
void PlayerCharacter::SetMoving(int i, bool v)
{
    moving[i] = v;
}
void PlayerCharacter::SetMoving(std::array<bool, 4> arr)
{
    moving = arr;
}
glm::vec3 PlayerCharacter::GetSpriteCenterLocation()
{
    return translation + glm::vec3(0.0, 48.0, 0.0);
}
glm::vec3 PlayerCharacter::GetTranslation()
{
    return translation;
}
void PlayerCharacter::SetTranslation(glm::vec3 t)
{
    translation = t;
}
void PlayerCharacter::SetTranslation(int i, float v)
{
    translation[i] = v;
}
glm::vec2 PlayerCharacter::GetCurrentSprite()
{
    return currentSprite;
}
void PlayerCharacter::CheckForIntersectionInSector(int yIndex, int xIndex)
{
    for (const auto& missileTarget : arena->sectors[yIndex][xIndex]) {
        auto mt = missileTarget.lock(); if (!mt) continue;
        if (CheckIntersection(mt) && mt->prop == 'e')
        {
            mt->shouldBeDeleted = 1;
            //std::cout << "Intersection!!\n";
        }
    }
}
bool PlayerCharacter::CheckIntersection(std::shared_ptr<Missile> missileB)
{
    glm::vec2 delta = GetSpriteCenterLocation() - missileB->texture->translation;
    float distanceSquared = glm::dot(delta, delta);
    float radiusSum = 32 + 32;

    if (distanceSquared <= radiusSum * radiusSum) {
        return true;
    }
    return false;
}
void PlayerCharacter::Move(glm::vec3 newPosition)
{
    if (translation == newPosition) return;
    int r = -1;

    if (translation.x > newPosition.x) r = 3;
    else if (translation.x < newPosition.x) r = 1;
    else if (translation.y > newPosition.y) r = 2;
    else if (translation.y < newPosition.y) r = 0;
    else r = -5;

    Rotate(r);
    if (translation.y < movementLimits.first) {
        translation.y = movementLimits.first;
        return;
    }
    if (translation.y > movementLimits.second) {
        translation.y = movementLimits.second;
        return;
    }
    translation = newPosition;
}
// not really used
void PlayerCharacter::Move(int direction, float distance)
{
    translation += glm::vec3((direction % 2 == 1) ? (direction / 2 == 0 ? 1 : -1) * distance : 0, direction % 2 == 0 ? (direction / 2 == 0 ? 1 : -1) * distance : 0, 0);
    Rotate(direction);
}
void PlayerCharacter::IterateAnimationSprite()
{
    //shader->SetUniformVec2f("u_texCoordOffSet", glm::vec2(0.0,0.5));
    if (currentSprite.x >= 1 - 1.0 / numOfSprites.x) currentSprite = glm::vec2(0.0, currentSprite.y);
    else currentSprite = glm::vec2(currentSprite.x + 1.0 / numOfSprites.x, currentSprite.y);
    shader->SetUniformVec2f("u_texCoordOffSet", currentSprite);
}
void PlayerCharacter::Update(std::chrono::milliseconds elapsedTime)
{
    if (GetMoving(0) || GetMoving(1) || GetMoving(2) || GetMoving(3)) {
        animationSwapTime += elapsedTime;
        //std::cout << "Animation swap: " << animationSwapTime << "\n";
        if (animationSwapTime >= std::chrono::milliseconds(200)) {
            animationSwapTime = std::chrono::milliseconds(0);
            IterateAnimationSprite();
        }
    }
    if (triggerChangeAnimationSprite) {
        shader->SetUniformVec2f("u_texCoordOffSet", currentSprite);
        triggerChangeAnimationSprite = 0;
    }
    if (!missilesTrigger.empty()) {
        for (const auto& [mousePos, speed] : missilesTrigger) {
            glm::vec2 direction = mousePos - glm::vec2(GetSpriteCenterLocation().x, GetSpriteCenterLocation().y);
            float w = abs(direction.x) + abs(direction.y);
            direction /= w;
            missiles.push_back(std::make_shared<Missile>(GetSpriteCenterLocation(), glm::vec3(direction, 0), speed, arena,'c'));
        }
        missilesTrigger.clear();
    }
    
    if (translation.x > Resolution::WIDTH) translation.x = Resolution::WIDTH;
    else if (translation.x < 0) translation.x = 0;
    if (translation.y > Resolution::HEIGHT) translation.y = Resolution::HEIGHT;
    else if (translation.y < 0) translation.y = 0;

    differenceInPosition = translation - prevTranslation;
    prevTranslation = translation;
    const float epsilon = 0.001f;
    
    moving = { false, false, false, false };

    if (differenceInPosition.y > epsilon) {
        moving[0] = true; // Up
    }
    else if (differenceInPosition.y < -epsilon) {
        moving[2] = true; // Down
    }

    if (differenceInPosition.x > epsilon) {
        moving[1] = true; // Right
    }
    else if (differenceInPosition.x < -epsilon) {
        moving[3] = true; // Left
    }
    if (!moving[0] && !moving[1] && !moving[2] && !moving[3]) {
        rotation = -5;
        currentSprite = glm::vec2(0, static_cast<float>(rotation) / numOfSprites.y);
    }
    for (auto it = missiles.begin(); it != missiles.end();) {
        if ((*it)->shouldBeDeleted) {
            it = missiles.erase(it);
        }
        else {
            (*it)->Update(elapsedTime);
            it++;
        }
    }
    int tmpResX = 1920, tmpResY = 1080;

    int ySectors = arena->sectors.size();
    int xSectors = arena->sectors[0].size();

    int yIndex = GetSpriteCenterLocation().y * ySectors / tmpResY;
    int xIndex = GetSpriteCenterLocation().x * xSectors / tmpResX;
    if (yIndex > ySectors - 1) yIndex = ySectors - 1;
    else if (yIndex < 0) yIndex = 0;
    if (xIndex > xSectors - 1) xIndex = xSectors - 1;
    else if (xIndex < 0) xIndex = 0;

    if (arena->sectors[yIndex][xIndex].size() > 0) {
        CheckForIntersectionInSector(yIndex, xIndex);
    }

    abilitySet->Update(elapsedTime);
}
void PlayerCharacter::Draw()
{
    Renderer renderer;

    glm::mat4 model(1.0f);
    model = glm::translate(model, translation);
    model = glm::translate(model, -Mesh::cameraTranslate);
    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, scale);
    //model = glm::scale(model, scaleCamera);
    mvp = Mesh::Proj * Mesh::View * model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(*vao, *ib, *shader);
    
    for (const auto& missile : missiles) {
        missile->Draw();
    }
    abilitySet->Draw();
}
void PlayerCharacter::DealDamage(std::shared_ptr<Player> player)
{
    //Audio::PlaySound("hurt");
}
bool PlayerCharacter::Rotate(int r)
{
    if (numOfSprites.y != 4) return false;
    if (abs(rotation - r) == 2) return false;
    if (rotation == r) currentSprite = glm::vec2(currentSprite.x, static_cast<float>(r) / numOfSprites.y);
    else if (!(moving[0] || moving[1] || moving[2] || moving[3])) currentSprite = glm::vec2(0, static_cast<float>(r) / numOfSprites.y);
    rotation = r;
    triggerChangeAnimationSprite = 1;
    return true;
}

