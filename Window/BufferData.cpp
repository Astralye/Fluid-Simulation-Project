#include "BufferData.h"

#include <iostream>

void CreateIndexBuffer(GLuint indexBuffer) {
	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;

	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	// Index Buffer 
	GLCall(glGenBuffers(1, &indexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

// Constructors ---------------------------------------------------------------
template<> BufferData<Vertex>::BufferData()
	: BufferType(VertexType::QUAD)
{

	m_Shader = std::make_unique<Shader>("res/shaders/quad.shader");
	m_Buffer = new Vertex[MaxVertexCount];

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(BufferType, MaxVertexCount);
	VertexBufferLayout layout;

	layout.Push<float>(4); // WorldPosition
	layout.Push<float>(4); // (R, G, B, A)
	m_VAO->AddBuffer(BufferType, *m_VBO, layout);

	CreateIndexBuffer(IndexBuffer);

	m_Shader->Bind();
}

template<> BufferData<CircleVertex>::BufferData()
	: BufferType(VertexType::CIRCLE)
{

	m_Shader = std::make_unique<Shader>("res/shaders/Circle.shader");
	m_Buffer = new CircleVertex[MaxVertexCount];

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(BufferType, MaxVertexCount);
	VertexBufferLayout layout;

	layout.Push<float>(4); // WorldPosition
	layout.Push<float>(4); // LocalPosition
	layout.Push<float>(4); // RGBA
	m_VAO->AddBuffer(BufferType, *m_VBO, layout);

	CreateIndexBuffer(IndexBuffer);
	m_Shader->Bind();
}

// Destructor -----------------------------------------------------------------
//template<typename T> BufferData<T>::~BufferData() {}

// End Batch ------------------------------------------------------------------
// These are all the same code
template<typename T> void BufferData<T>::EndBatch() {}
template<> void BufferData<Vertex>::EndBatch()
{
	GLsizeiptr size = (uint8_t*)m_BufferPtr - (uint8_t*)m_Buffer;
	m_VBO->Bind();

	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_Buffer));
}
template<> void BufferData<CircleVertex>::EndBatch()
{
	GLsizeiptr size = (uint8_t*)m_BufferPtr - (uint8_t*)m_Buffer;
	m_VBO->Bind();

	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_Buffer));
}

// Begin Batch ----------------------------------------------------------------
// These are all the same code
template<typename T> void BufferData<T>::BeginBatch() {}
template<> void BufferData<Vertex>::BeginBatch() {
	m_BufferPtr = m_Buffer;
}
template<> void BufferData<CircleVertex>::BeginBatch() {
	m_BufferPtr = m_Buffer;
}

// Flush Batch ---------------------------------------------------------------
// These are all the same code
template<typename T> void BufferData<T>::Flush() {}
template<> void BufferData<Vertex>::Flush() {
	m_Shader->Bind();
	m_VAO->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));
	IndexCount = 0;
}
template<> void BufferData<CircleVertex>::Flush() {

	m_Shader->Bind();
	m_VAO->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));
	IndexCount = 0;
}

// Draw Batch ----------------------------------------------------------------

// Draw Quads
template<typename T> void BufferData<T>::Draw(Rectangle& quad, glm::mat4 mvp) {}
template<> void BufferData<Vertex>::Draw(Rectangle& quad, glm::mat4 mvp) {

	if (IndexCount >= MaxIndexCount) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	for (int i = 0; i < 4; i++) {
		glm::vec4 vertex = mvp * glm::vec4(quad.m_Vertices[i], 0.0f, 1.0f);

		m_BufferPtr->WorldPosition = vertex;
		m_BufferPtr->Colour = { 0.2f, 0.2f, 0.2f, 1.0f };
		m_BufferPtr++;
	}
	IndexCount += 6;
}

// Draw circles
template<typename T> void BufferData<T>::Draw(Particle& particle, glm::mat4 mvp) {}
template<> void BufferData<CircleVertex>::Draw(Particle& particle, glm::mat4 mvp) {

	if (IndexCount >= MaxIndexCount) {
		EndBatch();
		Flush();
		BeginBatch();
	}

	glm::vec4 vertex;
	for (int i = 0; i < 4; i++) {

		vertex = mvp * glm::vec4(particle.m_Vertices[i], 0.0f, 1.0f);

		m_BufferPtr->WorldPosition = vertex;
		m_BufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
		m_BufferPtr->Colour = { 0.2f, 0.6f, 1.0f, 1.0f };
		m_BufferPtr++;
	}
	IndexCount += 6;
}