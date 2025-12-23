#pragma once
#include <map>
#include <utility>
#include <Button/Button.h>

class ButtonLookup
{
public:
	std::map<std::pair<int,int>, std::map<std::pair<int, int>, int>> table;
	ButtonLookup();
	void AddButton(int left, int top, int right, int bot, int buttonId);
	void AddButton(std::shared_ptr<Button> button);
	int GetButton(int x, int y);
};

