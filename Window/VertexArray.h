#pragma once

#include "Data_Structures.h"
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddCircleBuffer(const VertexBufferLayout& layout);
	void AddQuadBuffer(const VertexBufferLayout &layout);

	void AddBuffer(VertexType type,const VertexBuffer& vb, const VertexBufferLayout &layout);

	void Bind() const;
	void Unbind() const;
};