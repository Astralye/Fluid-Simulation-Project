#include "T4 - Calculate Density.h"

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
		time(0)
	{
		Initialize();
	}

	void T4_Calculate_Density::Initialize() {
		m_ParticleArray = new std::vector<Particle>;
		m_ParticleArray->reserve(Settings::MAX_PARTICLES);

		float radius = 1.0f;
		float spacing = 0.0f;

		Particle::init_Cube(m_ParticleArray, radius, spacing);
		//Particle::init_Random(m_ParticleArray, radius);
	}

	//Destructor
	T4_Calculate_Density::~T4_Calculate_Density(){}

	/*
	On update function runs on every frame.
	-	Updating each particle
	-	Updates projection matrix
	*/
	void T4_Calculate_Density::OnUpdate(){

		// This should always be run
		m_Proj = glm::ortho(camera.getProjection().left, camera.getProjection().right, camera.getProjection().bottom, camera.getProjection().top, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_MVP = m_Proj * m_View * m_Model;

		
		if (Settings::RECT_RESIZE) {
			m_RectContainer.update();
		}

		// If paused, does not update any values, is after MVP, to allow movement of camera
		if (Settings::PAUSE_SIMULATION) {
			return;
		}

		TIME(&SPH::CalculateAllDensities, m_ParticleArray, stats.Time_Calculate_Density);
		TIME(&SPH::CalculateAllPressures, m_ParticleArray, stats.Time_Calculate_Pressure);
		TIME(&SPH::CalculatePositionCollision, m_ParticleArray, m_RectContainer, stats.Time_Calculate_Movement);
		TIME(&SPH::CalculateAllViscosities, m_ParticleArray, stats.Time_Calculate_Viscosity);

		timeStep();
	}
	
	void T4_Calculate_Density::DrawCircle() {
		// Draw Particles
		for (int i = 0; i < m_ParticleArray->size(); i++) {
			CircleBuffer.Draw(m_ParticleArray->at(i), m_MVP);
		}
	}

	void T4_Calculate_Density::CreateContainer(RectangleContainer &rc) {
		QuadBuffer.Draw(rc.m_SideA, m_MVP);
		QuadBuffer.Draw(rc.m_SideB, m_MVP);
		QuadBuffer.Draw(rc.m_SideC, m_MVP);
		QuadBuffer.Draw(rc.m_SideD, m_MVP);
	}


	// Renders all particles
	// Renders the container
	void T4_Calculate_Density::OnRender()
	{
		// Set dynamic vertex buffer

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
				ImGui::Text("Draw calls: %i", stats.m_DrawCalls);
				ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);
				ImGui::Text("Per Frame: %.3fms", 1 / ImGui::GetIO().Framerate);
				ImGui::Text("Time: %.3f", time);

				ImGui::SeparatorText("Compute Times");
				ImGui::Text("Density: %.2fms", stats.Time_Calculate_Density.count() * 1000);
				ImGui::Text("Pressure: %.2fms", stats.Time_Calculate_Pressure.count() * 1000);
				ImGui::Text("Viscosity: %.2fms", stats.Time_Calculate_Viscosity.count() * 1000);
				ImGui::Text("Movement: %.2fms", stats.Time_Calculate_Movement.count() * 1000);
				ImGui::Text("Render: %.2fms", stats.Time_Render_Particles.count() * 1000);
			}

			if (ImGui::CollapsingHeader("SPH Config")) {
				ImGui::SliderFloat("Stiffness Constant:", &PhysicsEq::STIFFNESS_CONSTANT, 0.0f, 100000.0f);
				ImGui::SliderFloat("Rest Density:", &PhysicsEq::REST_DENSITY, 20.0f, 1.0f);
				ImGui::SliderFloat("Exponent value:", &PhysicsEq::EXPONENT, 1.0f, 10.0f);
				ImGui::SliderFloat("Viscosity:", &PhysicsEq::VISCOSITY, 0.0f, 1.0f);
			}

			if (ImGui::CollapsingHeader("Simulation Config")) {

				if (ImGui::TreeNode("Gravity")) {

					ImGui::Checkbox("Enable Gravity", &Settings::ENABLE_GRAVITY);
					if (!Settings::ENABLE_GRAVITY) { ImGui::BeginDisabled(); }
						ImGui::InputFloat("Acceleration constant:", &PhysicsEq::GRAVITY, 0.1f, 0.5f);
					if (!Settings::ENABLE_GRAVITY) { ImGui::EndDisabled(); }

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Particles")) {
					ImGui::Text("Max Particles:");
					ImGui::Text("Particles: ");
					ImGui::Text("Particle size: ");

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Container Config")) {

					ImGui::Checkbox("Modify Container", &Settings::RECT_RESIZE);

					if (!Settings::RECT_RESIZE) { ImGui::BeginDisabled(); }
						ImGui::SliderFloat("Width:", &m_RectContainer.m_Length, 70.0f, 300.0f);
						ImGui::SliderFloat("Height:", &m_RectContainer.m_Height, 70.0f, 300.0f);


						ImGui::SliderFloat("X:", &m_RectContainer.m_Position.x, -50.0f, 50.0f);
						ImGui::SliderFloat("Y:", &m_RectContainer.m_Position.y, -50.0f, 50.0f);

					if (!Settings::RECT_RESIZE) { ImGui::EndDisabled(); }

					ImGui::TreePop();
				}


			}

			if (ImGui::CollapsingHeader("DEBUG MENU")) {

				ImGui::Checkbox("Enable Debug mode", &Settings::ENABLE_DEBUG_MODE);

				static int mode = 0;

				if (!Settings::ENABLE_DEBUG_MODE) { ImGui::BeginDisabled(); }

					if (ImGui::RadioButton("Velocity", mode == Particle::DebugType::D_Velocity)){ mode = Particle::DebugType::D_Velocity; Particle::Debug = Particle::DebugType::D_Velocity; } ImGui::SameLine();
					if (ImGui::RadioButton("Density", mode == Particle::DebugType::D_Density)) { mode = Particle::DebugType::D_Density; Particle::Debug = Particle::DebugType::D_Density; } ImGui::SameLine();
					if (ImGui::RadioButton("Pressure", mode == Particle::DebugType::D_Pressure)) { mode = Particle::DebugType::D_Pressure; Particle::Debug = Particle::DebugType::D_Pressure; }
			
				if (!Settings::ENABLE_DEBUG_MODE) { ImGui::EndDisabled(); }

				ImGui::SeparatorText("UNIFORM ");

			}

			ImGui::End();
		}
	}

	inline void T4_Calculate_Density::timeStep() { time += SIMSTEP; }
	
	void T4_Calculate_Density::Shutdown() {
		delete[] m_ParticleArray;
	}
}