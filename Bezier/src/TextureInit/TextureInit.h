#pragma once
#include "OpenGL/Texture.h"
#include <memory>
#include <map>
class TextureInit
{
public:
	static std::map<std::string, unsigned int> textureMapSToI;
	static std::map<unsigned int, std::string> textureMapIToS;
	static std::shared_ptr<Texture> texture;
	static void InitTextures();
};

