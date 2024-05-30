#ifndef BUFFERDATA_H
#define BUFFERDATA_H

#include "Simulation/Settings.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include "Simulation/Particle.h"
#include "Simulation/Rectangle.h"

#include <memory>

void CreateIndexBuffer(GLuint indexBuffer);

template<typename T>
class BufferData {
	
	private:
		VertexType BufferType;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		GLuint IndexBuffer = 0;
		uint32_t IndexCount = 0;

		T* m_Buffer = nullptr;
		T* m_BufferPtr = nullptr;

		std::unique_ptr<Shader> m_Shader;

		glm::mat4 QuadVertexPositions = {
			//  X	  Y		Z	  W
			{ -0.5, -0.5f, 0.0f, 1.0f},
			{  0.5, -0.5f, 0.0f, 1.0f},
			{  0.5,  0.5f, 0.0f, 1.0f},
			{ -0.5,  0.5f, 0.0f, 1.0f}
		};

	public:

		//~BufferData();
		BufferData();

		void BeginBatch();
		void EndBatch();
		void Flush();

		void Draw(Rectangle &quad, glm::mat4 mvp);
		void Draw(Particle &particle, glm::mat4 mvp);
};

#endif