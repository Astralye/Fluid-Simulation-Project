#pragma once

#include "Misc/Data_Structures.h"

// Create a buffer for positions to be rendered
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(VertexType type, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};