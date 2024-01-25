#pragma once

#include "Test.h"

#include "Simulation/Particle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Sim.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>


namespace test {

	class FreeFallSim : public Test {
	public:
		FreeFallSim();
		~FreeFallSim();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		inline void timeStep();

	private:
		// Data sent to GPU
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		float m_ClearColour[4];

		Particle m_ParticleA;
		Particle m_ParticleB;
		Particle m_ParticleC;

		Particle m_ParticleD;
		Particle m_ParticleE;
		Particle m_ParticleF;
		Particle m_ParticleG;
		Particle m_ParticleH;
		Particle m_ParticleI;
		Particle m_ParticleJ;

		glm::mat4 m_Proj, m_View;


		// The smaller the simstep, the higher the accuracy, but will take longer.
		constexpr static float SIMSTEP = 0.016; // An integer is one second.

		// 30 FPS = 1/30 = 0.03
		// 60 FPS = 1/60 = 0.016
		// 100 FPS = 1/100 = 0.01

		float time;
	};
}