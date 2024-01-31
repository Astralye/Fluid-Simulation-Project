#pragma once

#include "Test.h"
#include "Settings.h"
#include "Data_Structures.h"

#include "Simulation/Collision.h"
#include "Simulation/Particle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Sim.h"
#include "Simulation/Rectangle.h"
#include "Shader.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"

#include <memory>

namespace test {

	class T4_Calculate_Density : public Test {

	private:
		enum ShaderProgram {
			CircleShader,
			QuadShader
		};

	public:
		T4_Calculate_Density();
		~T4_Calculate_Density();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void Shutdown() override;

		void BeginBatch();
		void EndBatch();
		void Flush(ShaderProgram shaderProg);

		void CreateQuad(Particle& p);
		void CreateQuad(Rectangle& r);
		void CreateContainer(RectangleContainer& rc);
		inline void timeStep();

	private:
		// Data sent to GPU
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::vector<Particle> m_ParticleArray;
		float m_ClearColour[4];

		std::unique_ptr<Shader> m_QuadShader;
		std::unique_ptr<Shader> m_CircleShader;

		Vertex* m_QuadBuffer = nullptr;
		Vertex* m_QuadBufferPtr = nullptr;
		uint32_t IndexCount = 0;

		GLuint QuadIB = 0;

		//Rectangle m_Rect1;
		RectangleContainer m_RectContainer;

		glm::mat4 m_Proj, m_View;

		// The smaller the simstep, the higher the accuracy, but will take longer.
		constexpr static float SIMSTEP = 0.016; // An integer is one second.
		int m_DrawCalls;

		// 30 FPS = 1/30 = 0.03
		// 60 FPS = 1/60 = 0.016
		// 100 FPS = 1/100 = 0.01

		float time;
	};
}