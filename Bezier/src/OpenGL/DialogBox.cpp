#include "DialogBox.h"
void DialogBox::SetText(const char* text)
{
	textRender = std::make_shared<Text>(80, 80 + 32 * 7, 32, 32, text, glm::vec3(1.0), Text::centered);
}
void DialogBox::SetUp(std::shared_ptr<Scene> parentScene)
{
	dialogBox = std::make_shared<TexturedRect>(40,40,Resolution::WIDTH - 80,Resolution::HEIGHT/3,"friendly");
}
void DialogBox::Update(float xpos, float ypos)
{
	
}

void DialogBox::Draw(bool fromChild)
{
	if (!shouldBeDrawn) return;
	if (dialogBox != nullptr)dialogBox->Draw();
	if (textRender != nullptr)textRender->Draw();
	/*for (const auto& pair : buttons)
	{
		pair.second->Draw();
	}*/
}

void DialogBox::PerformAction(int pressedButtonId)
{
	/*switch (pressedButtonId)
	{
	case GLFW_KEY_Z:
		buttons[currentActiveButtonIndex]->Activate();
		break;
	case GLFW_KEY_UP:
		if (buttons[currentActiveButtonIndex]->neighbours[0] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[0]->id;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (buttons[currentActiveButtonIndex]->neighbours[1] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[1]->id;
		}
		break;
	case GLFW_KEY_DOWN:
		if (buttons[currentActiveButtonIndex]->neighbours[2] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[2]->id;
		}
		break;
	case GLFW_KEY_LEFT:
		if (buttons[currentActiveButtonIndex]->neighbours[3] != NULL) {
			currentActiveButtonIndex = buttons[currentActiveButtonIndex]->neighbours[3]->id;
		}
		break;
	default:
		break;
	}*/
}

void DialogBox::PerformActionScroll(double ypos)
{
}

void DialogBox::ReceiveInfo(char* data)
{
}
