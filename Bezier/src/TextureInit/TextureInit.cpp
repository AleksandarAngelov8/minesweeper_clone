#include "TextureInit.h"

void TextureInit::InitTextures()
{
	// nauchi dali mogat da se unbindnat texturi
	// moje bi texture trqbva da e unique_ptr za da moje da se unbindvat texturi
	// dobavi unbind function v texture.cpp
	//PROMENI MAXA NA BROQ TEXTURI V Game.shader I V Mesh.cpp!!!!!
	texture->Bind();
}
std::shared_ptr<Texture> TextureInit::texture = std::make_shared<Texture>();
std::map<std::string, unsigned int> TextureInit::textureMapSToI;
std::map<unsigned int, std::string> TextureInit::textureMapIToS;