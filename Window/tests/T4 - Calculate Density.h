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

	public:
		T4_Calculate_Density();
		~T4_Calculate_Density();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void Shutdown() override;

		void BeginBatch();
		void EndBatch();
		void Flush();

		void DrawCircle(Particle& p, glm::mat4 MVP);
		void CreateQuad(Rectangle& r);
		void CreateContainer(RectangleContainer& rc);
		inline void timeStep();

	private:

		std::vector<Particle> m_ParticleArray;
		float m_ClearColour[4];

		glm::mat4 QuadVertexPositions = {
			//  X	  Y		Z	  W
			{ -0.5, -0.5f, 0.0f, 1.0f},
			{  0.5, -0.5f, 0.0f, 1.0f},
			{  0.5,  0.5f, 0.0f, 1.0f},
			{ -0.5,  0.5f, 0.0f, 1.0f}
		};

		// QUAD Data
		// ------------------------------------

		std::unique_ptr<VertexArray> m_QuadVAO;
		std::unique_ptr<VertexBuffer> m_QuadVertexBuffer;
		GLuint QuadIB = 0;

		Vertex* m_QuadBuffer = nullptr;
		Vertex* m_QuadBufferPtr = nullptr;

		uint32_t IndexCount = 0;

		std::unique_ptr<Shader> m_QuadShader;

		// Circle Data
		// ------------------------------------

		std::unique_ptr<VertexArray> m_CircleVAO;
		std::unique_ptr<VertexBuffer> m_CircleVertexBuffer;
		GLuint CircleIB = 0;

		CircleVertex* m_CircleBuffer = nullptr;
		CircleVertex* m_CircleBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;

		std::unique_ptr<Shader> m_CircleShader;

		// ------------------------------------

		VertexType drawType;
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