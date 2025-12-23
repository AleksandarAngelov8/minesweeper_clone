#include "VertexBuffer.h"
#include "Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : ptrToVertices(NULL)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::VertexBuffer()
    : ptrToVertices(NULL)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}
void VertexBuffer::Bind()const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind()const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
void VertexBuffer::CopyBuffer(const unsigned int& src, float* ptrToVert)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 5 * 80, ptrToVert, GL_STATIC_DRAW));
    /*glBindBuffer(GL_COPY_WRITE_BUFFER, m_RendererID);
    glBindBuffer(GL_COPY_READ_BUFFER, src);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 80 * sizeof(float));
    Bind();*/
}