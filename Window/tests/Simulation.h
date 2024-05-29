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
#include "UniformSpacePartition.h"

#include "SourceSink.h"

#include "Benchmark.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	class Simulation : public Test {

	public:
		Simulation();
		~Simulation();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;

		void allocArray();
		void resetArray();

		void Initialize();

		void DrawCircle();
		void CreateContainer(RectangleContainer& rc);
		void DrawGrid();

		void initParticleNo();

		inline void timeStep();

	private:

		glm::ivec2 USP_Grid;
		UniformSpacePartition m_USP;

		std::vector<Particle>* m_ParticleArray;
		float m_ClearColour[4];

		BufferData<Vertex> QuadBuffer;
		BufferData<CircleVertex> CircleBuffer;

		// ------------------------------------

		VertexType drawType;
		RectangleContainer m_RectContainer;

		glm::mat4 m_Proj, m_View, m_Model, m_MVP;

		Benchmark *benchmark;

		// ------------------------------------

		Source sourceA;
		uint16_t currentNumberParticles; // can store up to 65k values. 


		// The smaller the simstep, the higher the accuracy, but will take longer.
		constexpr static float SIMSTEP = 0.016f; // An integer is one second.

		// 30 FPS = 1/30 = 0.03
		// 60 FPS = 1/60 = 0.016
		// 100 FPS = 1/100 = 0.01

		float time;
	};
}