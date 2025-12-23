#include "Item.h"
#include <OpenGL/Resolution.h>
#include "Game/FloatingIndicator.h"
#include "Scene/Game/Game.h"

Item::Item(std::string label, int count, std::string texture, bool stackable)
	: label(label), count(count), texture(texture), stackable(stackable) {
	if (!stackable) {
		label = GetLabel(false);

		if (uniqueIndices.count(label) == 0) {
			uniqueIndices.emplace(label, -1);
		}
		uniqueIndices[label]++;
		this->label += std::to_string(uniqueIndices[label]);
		for (auto& label : uniqueIndices) {
			std::cout << label.first << " - " << label.second << "\n";
		}
		std::cout << "----------------------------------------------------------------\n";
	}
}

std::string Item::GetLabel(bool full)
{
	if (stackable || full) return label;
	std::string label = this->label;
	for (int i = 0; i < label.size(); i++) {
		if ('0' <= label[i] && label[i] <= '9') {
			label = label.substr(0, i);
			break;
		}
	}
	std::cout << "STRING:" << label << "\n";
	return label;
}


void Item::BindItemToCell(std::shared_ptr<Environment> environment, std::shared_ptr<Game> gameScene, std::shared_ptr<Cell> cell)
{
	environment->BindInteractableToCell(cell->position,shared_from_this(), [gameScene, this, cell]() {
		if (gameScene->character->inventory->AddItem(shared_from_this())) {
			gameScene->indicators.push_back(std::make_shared<FloatingIndicator>(Resolution::WIDTH / 2, Resolution::HEIGHT / 2, 24, 24, "+ " + GetLabel(), glm::vec3(0.0, 1.0, 0.0)));
			gameScene->environment->UnbindTopItemFromCell(cell->position,gameScene);
		}
		});
}

void Item::SetTextureInInventory(int x, int y, bool right) const
{
	inventoryTexture = std::make_shared<TexturedRect>((2 + right * 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + x * 128, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - y * 128 - 50, 96, 96, texture);
	countText = std::make_shared<Text>((2 + right * 3) * Resolution::WIDTH / 7 - 544 / 2 + 32 + x * 128 + 96 - 12, Resolution::HEIGHT / 2 + 544 / 2 - 96 - 32 - y * 128 + 12 - 50, 32, 32, std::to_string(count).c_str(), glm::vec3(1.0, 1.0, 1.0), Text::right);
}
std::map<std::string, int> Item::uniqueIndices;