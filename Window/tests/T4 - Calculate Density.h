#pragma once

#include <memory>
#include <imgui/imgui.h>

#include "Project_ImGui.h"

#include "BufferData.h"
#include "Test.h"
#include "Settings.h"
#include "Camera.h"
#include "Statistics.h"

#include "Simulation/Particle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Rectangle.h"
#include "SPH.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	class T4_Calculate_Density : public Test {

	public:
		T4_Calculate_Density();
		~T4_Calculate_Density();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void Shutdown() override;

		void Initialize();

		void DrawCircle();
		void CreateContainer(RectangleContainer& rc);
		inline void timeStep();

	private:

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

		// 30 FPS = 1/30 = 0.03
		// 60 FPS = 1/60 = 0.016
		// 100 FPS = 1/100 = 0.01

		float time;
	};
}