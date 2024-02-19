#include "T4 - Calculate Density.h"

#include "Camera.h"

#include <algorithm>

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>
#include <array>
#include <cstdlib>

// CAN PUT THIS IN ITS OWN CLASS
// INCLUDE 
// --> PARTICLE, VECTOR, CHRONO, RECTANGLECONTAINER
// 
// Couldn't use macro, using function pointers

void TIME(void (*func)(std::vector<Particle>*), std::vector<Particle>* particleArray, std::chrono::duration<float> &timer) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	func(particleArray);
	end = std::chrono::system_clock::now();
	timer = end - start;
}

void TIME(void (*func)(std::vector<Particle>*,RectangleContainer &container), std::vector<Particle>* particleArray, RectangleContainer& container, std::chrono::duration<float>& timer) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	func(particleArray,container);
	end = std::chrono::system_clock::now();
	timer = end - start;
}

namespace test {

	T4_Calculate_Density::T4_Calculate_Density()
		: m_Proj(glm::ortho(camera.getProjection().left, camera.getProjection().right,
			camera.getProjection().bottom, camera.getProjection().top, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_MVP(m_Proj * m_View * m_Model),

		m_ClearColour{ 1.0f, 1.0f, 1.0f, 1.0f },
		m_RectContainer(glm::vec3(0.0f, 50.0f, 0.0f),90.0f, 90.0f),
		drawType(VertexType::Null),
		m_DrawCalls(0),
		time(0)
	{
		Initialize();
	}

	void T4_Calculate_Density::Initialize() {
		m_ParticleArray = new std::vector<Particle>;
		m_ParticleArray->reserve(Settings::MAX_PARTICLES);

		// ------------------------------------------------------------

		float radius = 1.0f;
		float spacing = 0.0f;

		Particle::init_Cube(m_ParticleArray, radius, spacing);
		//Particle::init_Random(m_ParticleArray, radius);
	}

	//Destructor
	T4_Calculate_Density::~T4_Calculate_Density(){}

	/*On update function runs on every frame.
	- Updating each particle
	- Updates projection matrix
	- Checking and resolving for collisions (Would need to be optimized later)
	- (Not implemented) Resolve SPH values
	*/
	void T4_Calculate_Density::OnUpdate(float deltaTime){

		m_Proj = glm::ortho(camera.getProjection().left, camera.getProjection().right, camera.getProjection().bottom, camera.getProjection().right, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_MVP = m_Proj * m_View * m_Model;

		if (Settings::PAUSE_SIMULATION) {
			return;
		}

		TIME(&Particle::CalculateAllDensities, m_ParticleArray, m_Statistics.Time_Calculate_Density);
		TIME(&Particle::CalculateAllPressures, m_ParticleArray, m_Statistics.Time_Calculate_Pressure);
		TIME(&Particle::CalculatePositionCollision, m_ParticleArray, m_RectContainer, m_Statistics.Time_Calculate_Movement);
		TIME(&Particle::CalculateAllViscosities, m_ParticleArray, m_Statistics.Time_Calculate_Viscosity);

		timeStep();
	}
	
	//Function call for rendering all circles
	void T4_Calculate_Density::DrawCircle() {
		// Draw Particles
		for (int i = 0; i < m_ParticleArray->size(); i++) {
			CircleBuffer.Draw(m_ParticleArray->at(i), m_MVP);
		}
	}

	//Function call for rendering the container
	void T4_Calculate_Density::CreateContainer(RectangleContainer &rc) {
		QuadBuffer.Draw(rc.m_SideA, m_MVP);
		QuadBuffer.Draw(rc.m_SideB, m_MVP);
		QuadBuffer.Draw(rc.m_SideC, m_MVP);
		QuadBuffer.Draw(rc.m_SideD, m_MVP);
	}

	/* OnRender function runs after OnUpdate()
	- Renders all particles
	- Renders the container
	- Sets other OpenGL values
	*/
	void T4_Calculate_Density::OnRender()
	{
		// Set dynamic vertex buffer
		m_DrawCalls = 0;

		// Set Background
		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Set alpha
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Quad Batch Render
		{
			QuadBuffer.BeginBatch();
			CreateContainer(m_RectContainer);
			QuadBuffer.EndBatch();
			QuadBuffer.Flush();
		}


		// Circle Batch Render
		{
			CircleBuffer.BeginBatch();
			DrawCircle();
			CircleBuffer.EndBatch();
			CircleBuffer.Flush();
		}
	}

	// All the IMGui rendering to be displayed



	// This needs to be in its own file.
	void T4_Calculate_Density::OnImGuiRender()
	{	
		// Things to add:

		// - Menu bar

		// - Performance and statistics
		// - General overview of program.
		// - Help
		// - Modifications
		
		ImGui::End();
		{
			ImGui::Begin("", 0,
				ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			//if (ImGui::ArrowButton("##right -", ImGuiDir_Right)) {
			//	// Play
			//} ImGui::SameLine();
			const char* text;

			if (Settings::PAUSE_SIMULATION) {
				text = "Play";
			}
			else {
				text = "Pause";
			}


			if (ImGui::Button(text)) {

				if (Settings::PAUSE_SIMULATION) {
					Settings::PAUSE_SIMULATION = false;
				}
				else {
					Settings::PAUSE_SIMULATION = true;
				}

			} ImGui::SameLine();

			if (ImGui::Button("Restart")) {
				Initialize();
			} ImGui::SameLine();

			ImGui::End();
		}

		{
			ImGui::Begin("Misc", 0,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

			if (ImGui::CollapsingHeader("Controls")) {
				ImGui::Text("Move {W,A,S,D}");
				ImGui::Text("Zoom {-,+}");
			}

			if (ImGui::CollapsingHeader("Performance")) {

				ImGui::SeparatorText("General");
				ImGui::Text("Particles: %i", Settings::MAX_PARTICLES);
				ImGui::Text("Draw calls: %i", m_DrawCalls);
				ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
				ImGui::Text("Time: %.3f", time);

				ImGui::SeparatorText("Compute Times");
				ImGui::Text("Density: %.2fms", m_Statistics.Time_Calculate_Density.count() * 1000);
				ImGui::Text("Pressure: %.2fms", m_Statistics.Time_Calculate_Pressure.count() * 1000);
				ImGui::Text("Viscosity: %.2fms", m_Statistics.Time_Calculate_Viscosity.count() * 1000);
				ImGui::Text("Movement: %.2fms", m_Statistics.Time_Calculate_Movement.count() * 1000);
			}

			if (ImGui::CollapsingHeader("SPH Config")) {
				ImGui::SliderFloat("Stiffness Constant:", &PhysicsEq::STIFFNESS_CONSTANT, 0.0f, 100000.0f);
				ImGui::SliderFloat("Rest Density:", &PhysicsEq::REST_DENSITY, 20.0f, 1.0f);
				ImGui::SliderFloat("Exponent value:", &PhysicsEq::EXPONENT, 1.0f, 10.0f);
				ImGui::SliderFloat("Viscosity:", &PhysicsEq::VISCOSITY, 0.0f, 1.0f);
			}

			if (ImGui::CollapsingHeader("Simulation Config")) {

				ImGui::Checkbox("Enable Gravity", &Settings::ENABLE_GRAVITY);
				ImGui::InputFloat("Acceleration constant:", &PhysicsEq::GRAVITY, 0.1f,0.5f);
				ImGui::Text("Max Particles:");

			}

			if (ImGui::CollapsingHeader("DEBUG MENU")) {

				ImGui::Checkbox("Enable Debug mode", &Settings::ENABLE_DEBUG_MODE);

				static int mode = 0;

				if (ImGui::RadioButton("Velocity", mode == Particle::DebugType::D_Velocity)){ mode = Particle::DebugType::D_Velocity; Particle::Debug = Particle::DebugType::D_Velocity; } ImGui::SameLine();
				if (ImGui::RadioButton("Density", mode == Particle::DebugType::D_Density)) { mode = Particle::DebugType::D_Density; Particle::Debug = Particle::DebugType::D_Density; } ImGui::SameLine();
				if (ImGui::RadioButton("Pressure", mode == Particle::DebugType::D_Pressure)) { mode = Particle::DebugType::D_Pressure; Particle::Debug = Particle::DebugType::D_Pressure; }
			}

			ImGui::End();
		}
	}

	inline void T4_Calculate_Density::timeStep() { time += SIMSTEP; }
	
	void T4_Calculate_Density::Shutdown() {
		delete[] m_ParticleArray;
	}
}