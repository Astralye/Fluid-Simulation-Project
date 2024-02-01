#include "VertexBuffer.h"
#include "Renderer.h"

// size, total n size array
VertexBuffer::VertexBuffer(VertexType type, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    switch (type) {

        case VertexType::QUAD: {
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, nullptr, GL_DYNAMIC_DRAW));
            break;
        }

        case VertexType::CIRCLE: {
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(CircleVertex) * size, nullptr, GL_DYNAMIC_DRAW));
            break;
        }
    }
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void VertexBuffer::Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); }
void VertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
