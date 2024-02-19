#pragma once

#include "Test.h"
#include "Settings.h"
#include "Data_Structures.h"

#include "Simulation/Particle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Rectangle.h"
#include "Shader.h"

#include "BufferData.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"

#include "Project_ImGui.h"

#include <chrono>
#include <memory>

struct Statistics {
	std::chrono::duration<float> Time_Calculate_Density;
	std::chrono::duration<float> Time_Calculate_Pressure;
	std::chrono::duration<float> Time_Calculate_Movement;
	std::chrono::duration<float> Time_Calculate_Viscosity;
	std::chrono::duration<float> Time_Render_Particles;
};

namespace test {

	class T4_Calculate_Density : public Test {

	public:
		T4_Calculate_Density();
		~T4_Calculate_Density();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void Shutdown() override;

		void Initialize();

		void DrawCircle();
		void CreateContainer(RectangleContainer& rc);
		inline void timeStep();

	private:

		Statistics m_Statistics;

		std::vector<Particle>* m_ParticleArray;
		float m_ClearColour[4];

		BufferData<Vertex> QuadBuffer;
		BufferData<CircleVertex> CircleBuffer;

		// ------------------------------------

		VertexType drawType;
		RectangleContainer m_RectContainer;

		glm::mat4 m_Proj, m_View, m_Model, m_MVP;

		// The smaller the simstep, the higher the accuracy, but will take longer.
		constexpr static float SIMSTEP = 0.016f; // An integer is one second.
		int m_DrawCalls;

		// 30 FPS = 1/30 = 0.03
		// 60 FPS = 1/60 = 0.016
		// 100 FPS = 1/100 = 0.01

		float time;
	};
}