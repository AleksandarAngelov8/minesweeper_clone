#pragma once

#include "Renderer.h"

#include <string>
#include <vector>
class Texture
{
private:
	std::string m_FilePath;
	int m_Width, m_Height, m_BPP;
	unsigned char* m_LocalBuffer; 
	std::vector<unsigned int> m_RendererIDs; 
public:
	unsigned int GetTextureObject(unsigned int id);
	Texture();
	~Texture();
	
	void Bind();
	void Bind(const std::string& path) ;
	void Unbind() const;

	inline int GetWidth() const{ return m_Width; }
	inline int GetHeight() const{ return m_Height; }
};
