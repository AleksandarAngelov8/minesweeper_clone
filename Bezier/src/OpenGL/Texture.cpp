#include <iostream>
#include <filesystem>
#include <string>
#include "Texture.h"
#include "stb_image/stb_image.h"
#include <TextureInit/TextureInit.h>


unsigned int Texture::GetTextureObject(unsigned int id)
{
	if (id == m_RendererIDs.size()-1) {
		return m_RendererIDs[id];
	}
	return m_RendererIDs[m_RendererIDs.size()-2 - id];
}
//25 -> 25
//24 -> 0
//23 -> 1
//22 -> 2
//21 -> 3
//..
//0 -> 24
Texture::Texture()
	: m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
}

Texture::~Texture()
{
	
}

void Texture::Bind()
{
	std::string directory_path = "res\\textures";
	int i = 0;
	try {
		for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
			if (entry.path().extension() == ".png") {
				//std::cout << "Found PNG file: " << entry.path().stem().string() << std::endl;
				Bind(entry.path().stem().string());
				std::cout << i++ << "\t";
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
void Texture::Bind(const std::string& path)
{
	std::vector<unsigned int>::iterator it;

	std::string fullPath = "res/textures/" + path + ".png";

	unsigned int slot = m_RendererIDs.size();
	TextureInit::textureMapSToI.insert(std::make_pair(path, slot));
	TextureInit::textureMapIToS.insert(std::make_pair(slot, path));

	it = m_RendererIDs.begin();
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));	 
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(fullPath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	m_RendererIDs.insert(it, slot);
	GLCall(glGenTextures(1, &m_RendererIDs[slot]));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererIDs[slot]));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST););
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}


	glBindTextureUnit(slot, m_RendererIDs[slot]);
}

void Texture::Unbind() const
{
	
}