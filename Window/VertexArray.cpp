#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

// Data sent to GPU
struct Vertex {
	glm::vec2 Position;
	glm::vec4 Colour;
};

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto &elements = layout.GetElements();
	
	// Change interval when needed
	for (unsigned int i = 0; i < elements.size(); i+=2)
	{
		// Each element stores 2 data types, x,y in [0], R,G,B,A in [1], TexX,TexY in [2]
		// This code is automatic from layout.

		// Positional Vertex
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i,	 element.count, element.type,
			element.normalized, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

		// RGBA Vertex
		const auto& element2 = elements[i+1];
		GLCall(glEnableVertexAttribArray(i+1));
		GLCall(glVertexAttribPointer(i+1, element2.count, element2.type,
			element2.normalized, sizeof(Vertex), (const void*)offsetof(Vertex, Colour)));

		/*
		// Texture Coords Vertex
		const auto& element3 = elements[i + 2];
		GLCall(glEnableVertexAttribArray(i + 2));
		GLCall(glVertexAttribPointer(i + 2, element3.count, element3.type,
			element3.normalized, layout.GetStride(), (const void*)offset));

		offset += element3.count * VertexBufferElement::GetSizeOfType(element3.type);

		// Texture Coords Vertex
		const auto& element4 = elements[i + 3];
		GLCall(glEnableVertexAttribArray(i + 3));
		GLCall(glVertexAttribPointer(i + 3, element4.count, element4.type,
			element4.normalized, layout.GetStride(), (const void*)offset));

		offset += element4.count * VertexBufferElement::GetSizeOfType(element4.type);
		*/
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
