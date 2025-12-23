#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	float* ptrToVertices;
	void Bind()const;
	void Unbind()const;
	void CopyBuffer(const unsigned int& src, float* ptrToVert);
	inline unsigned int GetRendererID() const { return m_RendererID; }
};