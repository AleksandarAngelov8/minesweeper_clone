#include "ButtonLookup.h"
#include <iostream>
#include <OpenGL/Resolution.h>

ButtonLookup::ButtonLookup(){
}

void ButtonLookup::AddButton(int left, int top, int right, int bot, int buttonId) {
	if (table.count(std::make_pair(left, right)) > 0) {
		table[std::make_pair(left, right)].emplace(std::make_pair(top, bot), buttonId);
	}
	else {
		std::map<std::pair<int, int>, int> button;
		button.emplace(std::make_pair(top, bot), buttonId);
		table[std::make_pair(left, right)] = button;
	}
}

void ButtonLookup::AddButton(std::shared_ptr<Button> button)
{
	AddButton(button->minX,button->minY,button->maxX,button->maxY, button->id);
}

int ButtonLookup::GetButton(int x, int y)//258,122
{
	for (const auto& i1 : table) {
		if (i1.first.first <= x && i1.first.second >= x) {
			for (const auto& i2 : i1.second) {
				if (i2.first.first <= y && i2.first.second >= y) {
					return i2.second;
				}
			}
			return -1;
		}
	}
	return -1;
}
