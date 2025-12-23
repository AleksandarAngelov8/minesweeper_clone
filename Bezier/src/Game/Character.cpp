#include "Character.h"
#include <OpenGL/Resolution.h>
#include <Button/ButtonFunctionality.h>
#include <TextureInit/TextureInit.h>
#include "Game/Equipment.h"
#include "Game/Consumable.h"
#include "Scene/Game/Game.h"

#define cellSize LevelDataManagement::cellSize

Character::Character(float spawnX, float spawnY, float width, float height,
    std::string textureName, glm::vec2 numOfSprites, std::shared_ptr<Scene> scene,
    float maxhp, float currhp, float ad, unsigned int lvl, int exp) :
    scale(1.0, 1.0, 1.0), translation(0.0, 0.0, 0.0), rotation(0),
    movingUp(false), movingDown(false), movingRight(false), movingLeft(false),
    speed(4), currentSprite(glm::vec2(0.0, 0.0)), scene(scene), numOfSprites(numOfSprites),
    lockedInAnimation(-1), turnsToAttack(2), range(1)
{
    float texture = static_cast<float>(TextureInit::textureMapSToI[textureName]);
    float verticies[16]
    {
        -width / 2 + 3,   -8,           0.0, 0.0,
        width / 2 + 3,    -8,           1.0/numOfSprites.x, 0.0,
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
    translation += glm::vec3(spawnX + LevelDataManagement::cellSize/2, spawnY + LevelDataManagement::cellSize / 2, 0.0);
    Mesh::cameraTranslate = glm::vec3(spawnX,spawnY,0.0);
    Mesh::CenterCamera(spawnX,spawnY);

    //if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
    //    stats = std::make_shared<Stats>(maxhp, currhp, ad, lvl, exp, game);
    //    currentCell = game->environment->GetCell(GetPosY(false), GetPosX(false));
    //}

    inventory = std::make_shared<ItemContainer>();
    inventory->AddItem(std::make_shared<Item>("entranceKey", 1, "key", 0));
    inventory->AddItem(std::make_shared<Item>("exitKey", 1, "key", 0));
    inventory->AddItem(std::make_shared<Item>("slime", 3, "slime", 1));
    inventory->AddItem(std::make_shared<Consumable>("milk", 3, "milk", [this]() {
        //stats->AddHealth(5);
        }));
    inventory->AddItem(std::make_shared<Item>("chest", 2, "chest", 1));
    std::map<std::string, int> stIncr{
        std::make_pair("attackDamage",100)
    };
    inventory->AddItem(std::make_shared<Equipment>("dagger", "dagger", stIncr));
    //inventory->AddItem(std::make_shared<Equipment>("dagger", "dagger"));
    //inventory->AddItem(std::make_shared<Equipment>("dagger", "dagger"));
    //inventory->AddItem(std::make_shared<Equipment>("dagger", "dagger"));
}

void Character::ChargeAttack() {
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        
        int x1 = currentCell->position.first;
        int y1 = currentCell->position.second;
        int x2 = game->selectedCell->position.first;
        int y2 = game->selectedCell->position.second;
        
        int dx = x2 - x1;
        int dy = y2 - y1;

        int r = -1; // Default invalid direction

        if (abs(dx) > abs(dy)) {
            // Movement is more horizontal
            r = (dx > 0) ? 1 : 3; // Right if dx > 0, Left if dx < 0
        }
        else {
            // Movement is more vertical or equal
            r = (dy < 0) ? 2 : 0; // Down if dy > 0, Up if dy < 0
        }

        Rotate(r);

        if (std::abs((int)currentCell->position.first - (int)game->selectedCell->position.first) +
            std::abs((int)currentCell->position.second - (int)game->selectedCell->position.second) > range ||
            currentCell == game->selectedCell) {
            return;
        }
        if (attackingCell != nullptr && attackingCell != game->selectedCell) {
            InterruptAttack(attackingCell);
            lockedInAnimation = turnsToAttack;
        }
        attackingCell = game->selectedCell;

        if (lockedInAnimation == 0) {
            DealDamage(game->environment->cells[attackingCell->position.second][attackingCell->position.first]->enemy);
            game->environment->IterateWarningCell(attackingCell->position, lockedInAnimation, "Character");
            lockedInAnimation--;
            return;
        }
        if (lockedInAnimation == turnsToAttack) {
            game->environment->BindWarningToCell(attackingCell->position, "warning_signal_" + std::to_string(lockedInAnimation), []() {}, turnsToAttack, "Character");
        }
        else {
            game->environment->IterateWarningCell(attackingCell->position, lockedInAnimation, "Character");
        }
        lockedInAnimation--;
    }
}
void Character::InterruptAttack(std::shared_ptr<Cell> cell)
{
    if (!cell) return;
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        game->environment->IterateWarningCell(cell->position, lockedInAnimation, "Character", true);
        lockedInAnimation = -1;
    }
}
void Character::InterruptAttack()
{
    InterruptAttack(attackingCell);
}
void Character::IterateAnimationSprite()
{
    //shader->SetUniformVec2f("u_texCoordOffSet", glm::vec2(0.0,0.5));
    if (currentSprite.x >= 1-1.0/numOfSprites.x) currentSprite = glm::vec2(0.0, currentSprite.y);
    else currentSprite = glm::vec2(currentSprite.x+1.0/numOfSprites.x, currentSprite.y);
    shader->SetUniformVec2f("u_texCoordOffSet", currentSprite);
}
void Character::Update()
{
    //if (stats->currentHealth <= 0) {
    //    ButtonFunctionality::ChangeScene("gameOver");
    //    return;
    //}
    if (movingUp) {
        translation.y += speed;
        movingUp = ((int)translation.y < (int)target.y);
        //movingUp = translation.y != target.y || ButtonFunctionality::CheckPressedButton(GLFW_KEY_UP);
        //printf("%i != %i ? %i || %i\n", (int)translation.y, (int)target.y, (int)translation.y != (int)target.y, ButtonFunctionality::CheckPressedButton(GLFW_KEY_UP));
    }
    else if (movingDown) {
        translation.y -= speed;
        movingDown = ((int)translation.y > (int)target.y) ;
        //movingDown = translation.y != target.y || ButtonFunctionality::CheckPressedButton(GLFW_KEY_DOWN);
        //printf("%i != %i ? %i || %i\n", (int)translation.y, (int)target.y, (int)translation.y != (int)target.y, ButtonFunctionality::CheckPressedButton(GLFW_KEY_DOWN));
    }
    else if (movingRight) {
        translation.x += speed;
        movingRight = ((int)translation.x < (int)target.x);
        //movingRight = translation.x != target.x || ButtonFunctionality::CheckPressedButton(GLFW_KEY_RIGHT);
        //printf("%i != %i ? %i || %i\n", (int)translation.x, (int)target.x, (int)translation.x != (int)target.x, ButtonFunctionality::CheckPressedButton(GLFW_KEY_RIGHT));
    }
    else if (movingLeft) {
        translation.x -= speed;
        movingLeft = ((int)translation.x > (int)target.x);
        //movingLeft = translation.x != target.x || ButtonFunctionality::CheckPressedButton(GLFW_KEY_LEFT);
        //printf("%i != %i ? %i || %i\n", (int)translation.x, (int)target.x, (int)translation.x != (int)target.x, ButtonFunctionality::CheckPressedButton(GLFW_KEY_LEFT));
    }
    CheckForEnemies();

    /*movingUp = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_UP);
    movingDown = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_DOWN);
    movingRight = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_RIGHT);
    movingLeft = ButtonFunctionality::CheckPressedKeyboardButton(GLFW_KEY_LEFT);
    if (!movingUp && !movingRight && !movingDown && !movingLeft)
    {
        currentSprite = glm::vec2(0, currentSprite.y);
        shader->SetUniformVec1f("u_texCoordOffSet", currentSprite);
    }*/
}
void Character::Draw()
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
}
void Character::DealDamage(std::shared_ptr<Enemy> enemy)
{
    if (!enemy) return;
    //uint32_t damage = rand() % stats->attackDamage;
    //enemy->stats->currentHealth -= damage;
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        if (std::shared_ptr<Game> gameScene = std::dynamic_pointer_cast<Game>(scene); gameScene) {
            if (!gameScene->InputEnabled()) return;
            gameScene->lastClickTime = std::chrono::steady_clock::now();
        }
        float indicatorPosX, indicatorPosY;
        indicatorPosX = enemy->translation.x - Mesh::cameraTranslate.x;
        indicatorPosY = enemy->translation.y - Mesh::cameraTranslate.y;
        //game->indicators.push_back(std::make_shared<FloatingIndicator>(indicatorPosX, indicatorPosY + LevelDataManagement::cellSize, 48, 48, "-" + std::to_string(damage), glm::vec3(1.0, 0.0, 0.0)));
    }
    //Audio::PlaySound("hurt");
    if (!enemy->healthbar)enemy->CreateHealthbar();
}
void Character::Interact()
{
    if (std::shared_ptr<Game> gameScene = std::dynamic_pointer_cast<Game>(scene); gameScene) {
        if (!gameScene->InputEnabled()) return;
        gameScene->lastClickTime = std::chrono::steady_clock::now();
    }
    auto neighbour = currentCell->neighbours[rotation];
    if (currentCell->interactable) currentCell->Interact();
    else if (neighbour && neighbour->interactable && !neighbour->walkable)
        currentCell->neighbours[rotation]->Interact();
}
void Character::Stay()
{
    if (std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene)) {
        if (!game->movingEnemies)
            Game::tickUnits++;
    }
}
bool Character::UpCollision()
{
    return false;
}
bool Character::RightCollision()
{
    return false;
}
bool Character::DownCollision()
{
    return false;
}
bool Character::LeftCollision()
{
    return false;
}
void Character::Rotate(unsigned int r)
{
    if (numOfSprites.y != 4) return;
    if (rotation == r)
        currentSprite = glm::vec2(currentSprite.x, static_cast<float>(r) / numOfSprites.y);
    else currentSprite = glm::vec2(0, static_cast<float>(r) / numOfSprites.y);
    rotation = r;
    shader->SetUniformVec2f("u_texCoordOffSet", currentSprite);
}
unsigned int Character::GetPosX(bool ceil)
{
    double posX = translation.x / cellSize;
    if (ceil)
        posX = std::ceil(posX);
    else
        posX = std::floor(posX);

    return static_cast<unsigned int>(posX);
}
unsigned int Character::GetPosY(bool ceil)
{
    double posY = translation.y / cellSize;
    if (ceil)
        posY = std::ceil(posY);
    else
        posY = std::floor(posY);

    return static_cast<unsigned int>(posY);
}
void Character::StartMoveAnimation(uint32_t direction)
{
    std::shared_ptr<Game> game = std::dynamic_pointer_cast<Game>(scene);
    if (!game->InputEnabled()) return;
    if (game->movingEnemies)
        return;
    auto neighbour = currentCell->neighbours[direction];
    if (direction == UP && (movingRight || movingLeft || movingDown) ||
        direction == DOWN && (movingRight || movingLeft || movingUp) ||
        direction == RIGHT && (movingUp || movingLeft || movingDown) ||
        direction == LEFT && (movingRight || movingUp || movingDown)) return;

    if (!neighbour->walkable) {
        Rotate(direction);
        return;
    }

    glm::vec2 newTarget;
    switch (direction) {
    case UP:
        newTarget = glm::vec2(target.x, translation.y + 2 + (cellSize - (int)(translation.y + 2 + cellSize / 2) % static_cast<int>(cellSize)));
        break;
    case DOWN:
        newTarget = glm::vec2(target.x, translation.y - 2 - (int)(translation.y - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize));
        break;
    case RIGHT:
        newTarget = glm::vec2(translation.x + 2 + (cellSize - (int)(translation.x + 2 + cellSize / 2) % static_cast<int>(cellSize)), target.y);
        break;
    case LEFT:
        newTarget = glm::vec2(translation.x - 2 - (int)(translation.x - 2 - cellSize / 2 - cellSize) % static_cast<int>(cellSize), target.y);
        break;
    }
    if (newTarget == target) return;
    target = newTarget;
    Rotate(direction);
    switch (direction) {
    case UP:
        movingUp = true;
        break;
    case DOWN:
        movingDown = true; 
        break;
    case RIGHT:
        movingRight = true;
        break;
    case LEFT:
        movingLeft = true;
        break;
    }
    Game::tickUnits++;
    currentCell->character = nullptr;
    neighbour->character = shared_from_this();
    currentCell = neighbour;
    game->lastClickTime = std::chrono::steady_clock::now();
}
bool Character::MovingCheck(uint32_t direction)
{
    switch (direction)
    {
    case UP:
        return movingUp;
        break;
    case RIGHT:
        return movingRight;
        break;
    case DOWN:
        return movingDown;
        break;
    case LEFT:
        return movingLeft;
        break;
    }
    return false;
}
void Character::CheckForEnemies()
{
    for (int i = 0; i < 4; i++)
    {
        if (currentCell->neighbours[i]->enemy != nullptr && currentCell->neighbours[i]->enemy->alive) {
            //ButtonFunctionality::ChangeScene("combat");
            //std::shared_ptr<Scene> combatScene = ButtonFunctionality::getCurrentSceneObject();
            //std::dynamic_pointer_cast<Combat>(ButtonFunctionality::getCurrentSceneObject())->SetEnemies(shared_from_this(), neighbours[i]->enemy);
        }
    }
}
