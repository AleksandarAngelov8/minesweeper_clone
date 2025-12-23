#pragma once
#include "OpenGL/TexturedRect.h"
#include "Scene/Scene.h"
#include "OpenGL/Text.h"
class DialogBox : public Scene
{
	std::shared_ptr<TexturedRect> dialogBox;
	std::shared_ptr<Text> textRender;
public:
	bool shouldBeDrawn;
	int currentActiveButtonIndex;
	void SetText(const char* text);
	void SetUp(std::shared_ptr<Scene> parentScene = nullptr) override;
	void Update(float xpos, float ypos) override;
	void Draw(bool fromChild = 0) override;
	void PerformAction(int pressedButtonId) override;
	void PerformActionScroll(double ypos) override;
    void ReceiveInfo(char* data) override;
};
/*
graph TD
    subgraph Game Components
        Character --> Stats
        Character --> ItemContainer
        Character --> LevelDataManagement
        Enemy --> Character
        Enemy --> Stats
        Enemy --> Bar
        Enemy --> Scene
        NPC --> Quote
        Item --> TexturedRect
        ItemContainer --> Item
    end

    subgraph Scene Components
        Scene --> Button
        Scene --> Audio
        Scene --> Cursor
        Menu --> ButtonFunctionality
        Menu --> Scene
        Menu --> DialogBox
        Menu --> Character
        Game --> Scene
        Game --> Environment
        Game --> Character
        Game --> Enemy
        Game --> NPC
        Edit --> Scene
        Edit --> ButtonFunctionality
        ChestMenu --> Scene
        ChestMenu --> ItemContainer
        ItemMenu --> Scene
        ItemMenu --> ItemContainer
        ItemSelect --> Scene
        ItemSelect --> Character
        GameOver --> Scene
        NewMenu --> Scene
    end

    subgraph OpenGL Components
        Environment --> Cell
        Environment --> LevelDataManagement
        Environment --> TexturedRect
        DialogBox --> Scene
        DialogBox --> TexturedRect
        Mesh --> Cell
        TexturedRect --> Renderer
    end
*/

