#include "Enemy.h"
#include <OpenGL/Resolution.h>
#include <Button/ButtonFunctionality.h>
#include <TextureInit/TextureInit.h>
#include "Scene/Edit/Edit.h"
#include "Scene/Game/Game.h"

#define cellSize LevelDataManagement::cellSize

Enemy::Enemy(float spawnX, float spawnY, float width, float height,
    std::string textureName, unsigned int numOfSprites, float maxhp, float ad, unsigned int level, std::shared_ptr<Scene> scene, bool alive, std::string name) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
    currentSprite(glm::vec2(0.0, 0.0)), alive(true), scene(scene), visible(false),
    movingUp(false), movingDown(false), movingRight(false), movingLeft(false),
    speed(4.0), tickUnits(0), name(name), aggroed({ -1,-1 }), lockedInAnimation(-1), turnsToAttack(3)
{
    float texture = static_cast<float>(TextureInit::textureMapSToI[textureName]);
    vao = std::make_unique<VertexArray>();

    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        float verticies[16]
        {
            -width/2,   -height / 2, 0.0, 0.0,
             width / 2,    -height / 2, 1, 0.0,
             width / 2,    height/2 ,  1, 1,
            -width / 2,   height/2 ,  0.0, 1
        };
        vbo = std::make_unique<VertexBuffer>(verticies, 16 * sizeof(float));
        translation += glm::vec3(spawnY, spawnX, 0.0);
    }
    else if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
        float verticies[16]
        {
            spawnY + 0,spawnX + 0, 0.0, 0.0,
            spawnY + width,spawnX + 0, 1, 0.0,
            spawnY + width,spawnX + height ,  1, 1,
            spawnY + 0,spawnX + height ,  0.0, 1
        };
        vbo = std::make_unique<VertexBuffer>(verticies, 16 * sizeof(float));
    }

    unsigned int indicies[] = {
        0, 1, 2,
        0, 2, 3,
    };

    VertexBufferLayout layout1;
    layout1.Push<float>(2);
    layout1.Push<float>(2);
    vbo->Bind();
    vao->AddBuffer(*vbo, layout1);
    ib = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(int));

    shader = std::make_unique<Shader>("res/shaders/Enemy.shader");
    shader->Bind();
    shader->SetUniform1i("u_TexID", texture);

    translation += glm::vec3(width / 2, height / 2, 0.0);

    //stats = std::make_shared<Stats>(maxhp, maxhp, ad, level, 0, std::dynamic_pointer_cast<Game>(scene));
    if (!alive) Kill();
}
void Enemy::ChargeAttack()
{
    if (aggroed == std::make_pair((uint32_t)-1, (uint32_t)-1)) return;
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        if (lockedInAnimation == 0) {
            DamageCharacter();
            game->environment->IterateWarningCell(aggroed, lockedInAnimation, name);
            lockedInAnimation--;
            //game->environment->cells[aggroed.second][aggroed.first]->attackIn = lockedInAnimation;
            return;
        }
        if (lockedInAnimation == turnsToAttack) {
            game->environment->BindWarningToCell(aggroed, "warning_signal_" + std::to_string(lockedInAnimation), []() {}, turnsToAttack,name);
            //game->environment->cells[aggroed.second][aggroed.first]->attackIn = lockedInAnimation;
        }
        else {
            game->environment->IterateWarningCell(aggroed, lockedInAnimation, name);
        }
        lockedInAnimation--;
    }
}
void Enemy::Kill()
{
    if (!currentCell) return;
    alive = false;
    //character->stats->AddExperience(stats);
    //int exp = stats->ExpFromEnemy(character->stats->level);
    currentCell->SetInteraction([](){},true);
    currentCell->enemy = nullptr;
    currentCell->walkable = true;
    currentCell = nullptr;
    std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene);
    //game->indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2 + cellSize, Resolution::HEIGHT / 2, 32, 32, "+"+std::to_string(exp), glm::vec3(1.0, 0.0, 1.0)));

    auto attackingCell = game->environment->cells[aggroed.second][aggroed.first];

    int index = 0;
    for (int i = 1; i <= 3; i++) {
        if (attackingCell->attackWarnings[i].count(name) > 0) attackingCell->attackWarnings[i].erase(name);
        if (attackingCell->attackWarnings[i].size() > 0) index = i;
    }
    if (!index) attackingCell->textureObject = nullptr;
}
void Enemy::SetUpCellConnections()
{
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        currentCell = game->environment->GetCell(static_cast<unsigned int>(translation.y) / cellSize, static_cast<unsigned int>(translation.x) / cellSize);
        currentCell->walkable = false;
        currentCell->SetInteraction(
            [this]() {
                /*ButtonFunctionality::ChangeScene("combat");
                std::shared_ptr<Scene> combatScene = ButtonFunctionality::getCurrentSceneObject();
                std::dynamic_pointer_cast<Combat>(ButtonFunctionality::getCurrentSceneObject())->SetEnemies(std::dynamic_pointer_cast<Game>(scene)->character, shared_from_this());*/
            }
        ,false);
        if (!character) character = game->character;
    }
    else if (std::shared_ptr<Edit> edit = std::dynamic_pointer_cast<Edit>(scene)) {
        currentCell = edit->environment->GetCell(static_cast<unsigned int>(translation.y) / cellSize, static_cast<unsigned int>(translation.x) / cellSize);
    }
}
void Enemy::Move()
{
    if (lockedInAnimation > -1) {
        ChargeAttack();
        return;
    }
    if (!(currentCell->neighbours[UP]->walkable ||
        currentCell->neighbours[RIGHT]->walkable ||
        currentCell->neighbours[DOWN]->walkable ||
        currentCell->neighbours[LEFT]->walkable)) {
        return;
    }
    direction = 0;
    //std::stack<std::pair<std::shared_ptr<Cell>, int>>& searchingStack
    std::shared_ptr<Cell> nextCell;
    int xDif, yDif;
    xDif = std::abs(int(currentCell->position.second) - int(character->currentCell->position.second));
    yDif = std::abs(int(currentCell->position.first) - int(character->currentCell->position.first));
// LOOK IN CHATGPT ANSWER
    if (xDif + yDif <= 6) {
        std::cout << xDif << "/" << yDif << "\n";
    
        nextCell = character->currentCell->FindCharacter(character->currentCell, currentCell, 6);
    }
   

    do {
        direction = rand() % 4;
    } while (!currentCell->neighbours[direction]->walkable);

    if (nextCell) {
        auto pos = nextCell->position;
        if (pos == character->currentCell->position) {
            lockedInAnimation = turnsToAttack;
            ChargeAttack();
            return;
        }
        else if (currentCell->position.first > pos.first) direction = LEFT;
        else if (currentCell->position.first < pos.first) direction = RIGHT;
        else if (currentCell->position.second > pos.second) direction = DOWN;
        else if (currentCell->position.second < pos.second) direction = UP;
        else {
        }
    }
    else {
        //std::cout << name << " doesn't find character.\n";
    }
    switch (direction) {
    case UP:
        if (movingRight || movingLeft || movingDown || !currentCell->neighbours[UP]->walkable) return;
        target = glm::vec2(target.x, translation.y + (cellSize - (int)(translation.y + 2 + cellSize / 2) % static_cast<int>(cellSize)));
        Rotate(UP);
        movingUp = true;
        break;
    case RIGHT:
        if (movingUp || movingLeft || movingDown || !currentCell->neighbours[RIGHT]->walkable) return;
        target = glm::vec2(translation.x + (cellSize - (int)(translation.x + 2 + cellSize / 2) % static_cast<int>(cellSize)), target.y);
        Rotate(RIGHT);
        movingRight = true;
        break;
    case DOWN:
        if (movingRight || movingLeft || movingUp || !currentCell->neighbours[DOWN]->walkable) return;
        target = glm::vec2(target.x, translation.y - (int)(translation.y - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize));
        Rotate(DOWN);
        movingDown = true;
        break;
    case LEFT:
        if (movingRight || movingUp || movingDown || !currentCell->neighbours[LEFT]->walkable) return;
        target = glm::vec2(translation.x - (int)(translation.x - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize), target.y);
        Rotate(LEFT);
        movingLeft = true;
        break;
    }
    /*
    if (aggroed != std::make_pair((uint32_t)-1, (uint32_t)-1)) {

        if (aggroed == character->currentCell->position) {
            lockedInAnimation = turnsToAttack;
            ChargeAttack();
            return;
        }
        else if (currentCell->position.first > aggroed.first) direction = LEFT;
        else if (currentCell->position.first < aggroed.first) direction = RIGHT;
        else if (currentCell->position.second > aggroed.second) direction = DOWN;
        else if (currentCell->position.second < aggroed.second) direction = UP;
        else {
        }
    }
    switch (direction) {
    case UP:
        if (movingRight || movingLeft || movingDown || !currentCell->neighbours[UP]->walkable) return;
        target = glm::vec2(target.x, translation.y + (cellSize - (int)(translation.y + 2 + cellSize / 2) % static_cast<int>(cellSize)));
        Rotate(UP);
        movingUp = true;
        break;
    case RIGHT:
        if (movingUp || movingLeft || movingDown || !currentCell->neighbours[RIGHT]->walkable) return;
        target = glm::vec2(translation.x + (cellSize - (int)(translation.x + 2 + cellSize / 2) % static_cast<int>(cellSize)), target.y);
        Rotate(RIGHT);
        movingRight = true;
        break;
    case DOWN:
        if (movingRight || movingLeft || movingUp || !currentCell->neighbours[DOWN]->walkable) return;
        target = glm::vec2(target.x, translation.y - (int)(translation.y - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize));
        Rotate(DOWN);
        movingDown = true;
        break;
    case LEFT:
        if (movingRight || movingUp || movingDown || !currentCell->neighbours[LEFT]->walkable) return;
        target = glm::vec2(translation.x - (int)(translation.x - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize), target.y);
        Rotate(LEFT);
        movingLeft = true;
        break;
    }*/

    currentCell->neighbours[direction]->walkable = false;
    currentCell->neighbours[direction]->enemy = currentCell->enemy;
    currentCell->walkable = true;
    currentCell->enemy = nullptr;
    currentCell = currentCell->neighbours[direction];
}
void Enemy::Rotate(unsigned int r)
{
    rotation = r;
    currentSprite = glm::vec2(currentSprite.x, static_cast<float>(r) / 4);
}
void Enemy::DamageCharacter()
{
    //if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
    //    auto targetedCell = game->environment->cells[aggroed.second][aggroed.first];
    //    uint32_t damage = rand() % stats->attackDamage + 5;
    //    if (targetedCell->character != nullptr) {
    //        character->stats->currentHealth -= damage;
    //        game->indicators.push_back(std::make_shared<FloatingIndicator>(
    //            Resolution::WIDTH / 2, 
    //            LevelDataManagement::cellSize + Resolution::HEIGHT / 2
    //            , 48, 48, "-" + std::to_string(damage), glm::vec3(1.0, 0.0, 0.0)));
    //    }
    //    else if (targetedCell->enemy != nullptr) {
    //        targetedCell->enemy->stats->currentHealth -= damage;
    //        float indicatorPosX, indicatorPosY;
    //        indicatorPosX = targetedCell->enemy->translation.x - Mesh::cameraTranslate.x;
    //        indicatorPosY = targetedCell->enemy->translation.y - Mesh::cameraTranslate.y;
    //        game->indicators.push_back(std::make_shared<FloatingIndicator>(
    //            indicatorPosX,
    //            indicatorPosY
    //            , 48, 48, "-" + std::to_string(damage), glm::vec3(1.0, 0.0, 0.0)));
    //        if (!targetedCell->enemy->healthbar)targetedCell->enemy->CreateHealthbar();
    //    }
    //}
    //Audio::PlaySound("hurt");
}
bool Enemy::ShouldMove()
{
    return (abs(translation.x - character->translation.x) <= cellSize * 10 &&
        abs(translation.y - character->translation.y) <= cellSize * 10);
}
void Enemy::CreateHealthbar()
{
    //healthbar = std::make_shared<Bar>(0, 0, 96, 20, &stats->maxHealth, &stats->currentHealth, glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0), true);
}
void Enemy::Update()
{
    // control speed of movement
    //if (stats->currentHealth <= 0) {
    //    Kill();
    //}
    if (!alive) return;
    if (tickUnits < Game::tickUnits ) {
        Move();
        tickUnits=Game::tickUnits;
    }
    if (movingUp) {
        translation.y += speed;
        movingUp = ((int)translation.y < (int)target.y);
    }
    else if (movingDown) {
        translation.y -= speed;
        movingDown = ((int)translation.y > (int)target.y);
    }
    else if (movingRight) {
        translation.x += speed;
        movingRight = ((int)translation.x < (int)target.x);
    }
    else if (movingLeft) {
        translation.x -= speed;
        movingLeft = ((int)translation.x > (int)target.x);
    }
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)){
        if (movingUp || movingRight || movingDown || movingLeft) {
            game->movingEnemies = 1;
        }
    }

    if (healthbar)healthbar->Update(translation.x - 48, translation.y + 48);
}
void Enemy::Draw()
{
    if (!alive || !visible) return;
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

    if(healthbar)healthbar->Draw();
}
