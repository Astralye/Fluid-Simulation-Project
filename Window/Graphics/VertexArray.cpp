#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

// Class unspecific
VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &m_RendererID)); }
VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_RendererID)); }
void VertexArray::Bind() const { GLCall(glBindVertexArray(m_RendererID)); }
void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }

// Add buffers
// -------------------------
void VertexArray::AddCircleBuffer(const VertexBufferLayout& layout) {
	// Buffer for Circles

	const auto& elements = layout.GetElements();
	int noElements = 3;

	for (unsigned int i = 0; i < elements.size(); i += noElements)
	{
		// WorldPosition
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, WorldPosition)));

		// LocalPosition
		const auto& element1 = elements[i + 1];
		GLCall(glEnableVertexAttribArray(i + 1));
		GLCall(glVertexAttribPointer(i + 1, element1.count, element1.type,
			element1.normalized, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, LocalPosition)));

		// RGBA Vertex
		const auto& element2 = elements[i + 2];
		GLCall(glEnableVertexAttribArray(i + 2));
		GLCall(glVertexAttribPointer(i + 2, element2.count, element2.type,
			element2.normalized, sizeof(CircleVertex), (const void*)offsetof(CircleVertex, Colour)));
	}
}

void VertexArray::AddQuadBuffer(const VertexBufferLayout &layout) {
	
	const auto& elements = layout.GetElements();
	int noElements = 2;

	for (unsigned int i = 0; i < elements.size(); i += noElements)
	{
		// WorldPosition
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, sizeof(Vertex), (const void*)offsetof(Vertex, WorldPosition)));

		// RGBA Vertex
		const auto& element2 = elements[i + 1];
		GLCall(glEnableVertexAttribArray(i + 1));
		GLCall(glVertexAttribPointer(i + 1, element2.count, element2.type,
			element2.normalized, sizeof(Vertex), (const void*)offsetof(Vertex, Colour)));
	}
}

void VertexArray::AddBuffer(VertexType type, const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	
	switch (type) {

		case VertexType::QUAD: {
			AddQuadBuffer(layout);
			break;
		}

		case VertexType::CIRCLE: {
			AddCircleBuffer(layout);
			break;
		}
	}
}
