#include "T4 - Calculate Density.h"

namespace test {

	T4_Calculate_Density::T4_Calculate_Density()
		: m_Proj(glm::ortho(camera.getProjection().left, camera.getProjection().right,
			camera.getProjection().bottom, camera.getProjection().top, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_MVP(m_Proj * m_View * m_Model),

		m_ClearColour{ 1.0f, 1.0f, 1.0f, 1.0f },
		m_RectContainer(glm::vec3(0.0f, 70.0f, 0.0f),100.0f, 100.0f),
		USP_Grid({5,5}),

		drawType(VertexType::Null),
		time(0)
	{
		allocArray();
		m_USP.SetContainer(m_RectContainer, USP_Grid);
	}


	// Only gets run once
	void T4_Calculate_Density::allocArray()
	{
		m_ParticleArray = new std::vector<Particle>;
		m_USP.InitializeLookup();

		Initialize();
	}
	
	// Runs for resetting the simulation
	void T4_Calculate_Density::resetArray() {
		m_ParticleArray->clear();
		m_USP.lookupList->clear();
		m_USP.startIndex->clear();

		Initialize();
	}

	// Run at initialization and resetting.
	void T4_Calculate_Density::Initialize() {
		/* 
			m_ParticleArray contains ALL the particles.
			This should be used to allocate memory for a max number
			and not for comparisions and or collision detections.
		*/

		Settings::INIT_SIM = true;

		Settings::CURRENT_PARTICLES = 1000;
		currentNumberParticles = 1000;

		initParticleNo();

		//Particle::init_Random(m_ParticleArray, radius, currentNumberParticles);


		// Generates the Source
		sourceA = Source({ 0.0f,0.0f,0.0f }, 1, 5);
	}

	void T4_Calculate_Density::initParticleNo() {

		std::cout << "here" << std::endl;

		Particle::PARTICLE_RADIUS = 0.5f;
		float spacing = 0.0f;
		Particle::init_Cube(m_ParticleArray, spacing, Settings::CURRENT_PARTICLES);
	}

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

		if (Settings::ENABLE_RESIZE_CONTAINER) {
			m_RectContainer.update();
			m_USP.SetContainer(m_RectContainer);
		}

		if (Settings::INIT_SIM) {
			// Maybe only run when the values have changed
			if (currentNumberParticles != Settings::CURRENT_PARTICLES) {
				currentNumberParticles = Settings::CURRENT_PARTICLES;
				//initParticleNo();
			}

		}

		if (Settings::ENABLE_SOURCE) {
			sourceA.update(); // Update location
		}

		// This works if the simulation is already paused.
		if (Settings::START_BENCHMARK) {

			// Creates Benchmark once
			if (Settings::CREATE_BENCHMARK) {
				Settings::CREATE_BENCHMARK = false;

				benchmark = new Benchmark(0.033, 10);
			}

			benchmark->run(ImGui::GetIO().Framerate,
				{ 
				stats.Time_Create_Lookup.count() * 1000,
				stats.Time_Sort_Lookup.count() * 1000,
				stats.Time_Neighbour_Cells.count() * 1000,
				stats.Time_Render_Particles.count() * 1000
				});

			// Check if at the end of the benchmark
			if (!benchmark->isStillBenchmark()) {
				delete benchmark;
				Settings::START_BENCHMARK = false;
			}
		}

		// If paused, does not update any values, is after MVP, to allow movement of camera
		if (Settings::PAUSE_SIMULATION) { return; }

		Settings::INIT_SIM = false;
		// SPH CALCULATIONS 

		m_USP.checkPartition(m_ParticleArray,m_RectContainer);
		m_USP.getNeighbourParticles(m_ParticleArray, m_RectContainer);

		//TIME(&m_USP.checkPartition, m_ParticleArray, m_RectContainer);
		
		
		// Naive method, loop through all particles
		//TIME(&SPH::CalculateAllDensities, m_ParticleArray, stats.Time_Calculate_Density);
		//TIME(&SPH::CalculateAllPressures, m_ParticleArray, stats.Time_Calculate_Pressure);
		//TIME(&SPH::CalculatePositionCollision, m_ParticleArray, m_RectContainer, stats.Time_Calculate_Movement);
		//TIME(&SPH::CalculateAllViscosities, m_ParticleArray, stats.Time_Calculate_Viscosity);


		// Source generation would be evaluated here.
		if (Settings::ENABLE_SOURCE) { sourceA.addParticle(currentNumberParticles, m_ParticleArray); }

		timeStep();
	}
	
	void T4_Calculate_Density::DrawCircle() {

		// This is not really efficient
		// Could perhaps make this faster via cuda

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

	void T4_Calculate_Density::DrawGrid() {
		
		// Copy contents
		Rectangle tmpRect = m_USP.m_RenderSquare;
		glm::vec3 offset = { m_USP.m_CellSize.x, m_USP.m_CellSize.y, 0.0f };
		glm::vec3 origin = tmpRect.m_Position;

		int position = 0;

		glm::vec3 colourA = { 1.0f, 1.0f, 1.0f };
		glm::vec3 colourB = { 0.95f, 0.95f, 0.95f };
		glm::vec3 tmp;

		for (int i = 0; i < m_USP.m_Cells.y; i++) {

			for (int j = 0; j < m_USP.m_Cells.x; j++) {

				if (position % 2 == 0) { tmpRect.m_Colour = colourA;}
				else { tmpRect.m_Colour = colourB; }

				tmpRect.update_Position({ 
					origin.x + (offset.x * j), 
					origin.y + (offset.y * i), 0.0f });

				tmpRect.update_Vertices();
				QuadBuffer.Draw(tmpRect, m_MVP);

				position++;
			}

			// Swap colour
			if (m_USP.m_Cells.x % 2 == 0) {
				tmp = colourA;
				colourA = colourB;
				colourB = tmp;
			}

		}

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
			// Partition background
			if (Settings::ENABLE_PARTITION_BACKROUND && Settings::ENABLE_DEBUG_MODE) {
				QuadBuffer.BeginBatch();
				DrawGrid();
				QuadBuffer.EndBatch();
				QuadBuffer.Flush();
			}

			// Container Boxes
			QuadBuffer.BeginBatch();
			
			CreateContainer(m_RectContainer);

			if (Settings::ENABLE_SOURCE) {
				QuadBuffer.Draw(sourceA.m_Quad, m_MVP);
			}

			QuadBuffer.EndBatch();
			QuadBuffer.Flush();

		}

		// Circle Batch Render
		{
			CircleBuffer.BeginBatch();
			DrawCircle();

			// If hovered
			if (Settings::IS_HOVER_PARTICLE && Settings::ENABLE_DEBUG_MODE) {
				Particle particle = m_ParticleArray->at(Settings::HOVER_PARTICLE);
				particle.setRadius(2 * particle.getRadius());
				particle.update();

				CircleBuffer.Draw(particle, m_MVP);
			}

			// If clicked,
			if ((Settings::CLICK_PARTICLE != -1) && Settings::ENABLE_DEBUG_MODE) {
				Particle particle = m_ParticleArray->at(Settings::CLICK_PARTICLE);
				particle.setRadius(2 * particle.getRadius());
				particle.update();

				CircleBuffer.Draw(particle, m_MVP);
			}

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
		
		{
			ImGui::Begin("Hello", 0,
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
				resetArray();
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
				ImGui::Text("Per Frame: %.3fms", ( 1 / ImGui::GetIO().Framerate ) * 1000);
				ImGui::Text("Time: %.3f", time);

				ImGui::SeparatorText("Compute Times");
				ImGui::Text("Create Lookup Table: %.2fms", stats.Time_Create_Lookup.count() * 1000);
				ImGui::Text("Sort Lookup: %.2fms", stats.Time_Sort_Lookup.count() * 1000);
				ImGui::Text("SPH neighbour cells: %.2fms", stats.Time_Neighbour_Cells.count() * 1000);
				ImGui::Text("Render: %.2fms", stats.Time_Render_Particles.count() * 1000);
				ImGui::SeparatorText("SPH process times");

				ImGui::Text("Density: %.2fms", stats.Time_Calculate_Density.count() * 1000);
				ImGui::Text("Pressure: %.2fms", stats.Time_Calculate_Pressure.count() * 1000);
				ImGui::Text("Viscosity: %.2fms", stats.Time_Calculate_Viscosity.count() * 1000);
				ImGui::Text("Movement: %.2fms", stats.Time_Calculate_Movement.count() * 1000);
				
				if (ImGui::Button("Benchmark") && !Settings::START_BENCHMARK) {
					// Reset simulation
					resetArray();
					Settings::PAUSE_SIMULATION = false;
					// Start timer

					Settings::START_BENCHMARK = true;
					Settings::CREATE_BENCHMARK = true;

				}
				
				// Code for the bench mark is elsewhere.
				// This is only for displaying the GUI.
				if (Settings::START_BENCHMARK) {
					ImGui::Text("Running Benchmark...");
				}


				ImGui::SeparatorText("Compute Times");
			}

			if (ImGui::CollapsingHeader("SPH Config")) {
				ImGui::SliderFloat("Kernel Radius:", &Particle::KERNEL_RADIUS, 0.0f, 10.0f);

				ImGui::SliderFloat("Bounce CoEfficient:", &PhysicsEq::BOUNCE_COEFF, 0.0f, 1.0f);
				ImGui::SliderFloat("Stiffness Constant:", &PhysicsEq::STIFFNESS_CONSTANT, 0.0f, 10000.0f);
				ImGui::SliderFloat("Target Density:", &PhysicsEq::REST_DENSITY, 1.0f, 30.0f);
				ImGui::SliderFloat("Exponent value:", &PhysicsEq::EXPONENT, 1.0f, 20.0f);
				ImGui::SliderFloat("Viscosity:", &PhysicsEq::VISCOSITY, 0.0f, 0.7f);

			}

			if (ImGui::CollapsingHeader("Simulation Config")) {

				if (ImGui::TreeNode("Gravity")) {

					ImGui::Checkbox("Enable Gravity", &Settings::ENABLE_GRAVITY);
					ImGui::Separator();

					if (!Settings::ENABLE_GRAVITY) { ImGui::BeginDisabled(); }
						ImGui::InputFloat("Acceleration constant:", &PhysicsEq::GRAVITY, 0.1f, 0.5f);
					if (!Settings::ENABLE_GRAVITY) { ImGui::EndDisabled(); }

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Particles")) {

					ImGui::Text("Max Particles: %i", Settings::MAX_PARTICLES);
					ImGui::Text("Particles: %i", currentNumberParticles);

					ImGui::SliderInt("Starting Particles", &Settings::CURRENT_PARTICLES, 1, Settings::MAX_PARTICLES);
					ImGui::SliderFloat("Width:", &m_RectContainer.m_Length, 70.0f, 300.0f);

					//ImGui::Text("Particle radius: %i", m_ParticleArray->at(0).getRadius());
					
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Container Config")) {

					ImGui::Checkbox("Modify Container", &Settings::ENABLE_RESIZE_CONTAINER);

					if (!Settings::ENABLE_RESIZE_CONTAINER) { ImGui::BeginDisabled(); }
					
					ImGui::SliderFloat("Width:", &m_RectContainer.m_Length, 70.0f, 300.0f);
					ImGui::SliderFloat("Height:", &m_RectContainer.m_Height, 70.0f, 300.0f);

					ImGui::SliderFloat("X:", &m_RectContainer.m_Position.x, -500.0f, 500.0f);
					ImGui::SliderFloat("Y:", &m_RectContainer.m_Position.y, -500.0f, 500.0f);


					ImGui::SeparatorText("UNIFORM SPACE PARTITIONING");

					ImGui::Text("Cell Dimensions: {%i x %i}", m_USP.m_Cells.x, m_USP.m_Cells.y);
					ImGui::Text("Cell Start: {%.1f, %.1f}", m_USP.getPosition().x, m_USP.getPosition().y);
					ImGui::Text("Cell Size: {%.1f x %.1f}", m_USP.m_CellSize.x, m_USP.m_CellSize.y);

					if (ImGui::Button("Kernel Cell Size")) {
						m_USP.defaultSize(m_RectContainer, { Particle::KERNEL_RADIUS,Particle::KERNEL_RADIUS });
					}

					ImGui::Checkbox("Square Partitions", &Settings::ENABLE_SQUARE_PARTITIONS);
						
					if (Settings::ENABLE_SQUARE_PARTITIONS) {
						ImGui::SliderInt("Cell Size:", &Settings::PARITIONS_SIZE, 2, 50);
						m_USP.defaultSize(m_RectContainer, { Settings::PARITIONS_SIZE,Settings::PARITIONS_SIZE });
					}
					else {
						ImGui::SliderInt("X cells", &m_USP.m_Cells.x, 2, 50);
						ImGui::SliderInt("Y cells", &m_USP.m_Cells.y, 2, 50);
					}

					if (!Settings::ENABLE_RESIZE_CONTAINER) { ImGui::EndDisabled(); }

					ImGui::TreePop();
				}

				// Water Flow
				if (ImGui::TreeNode("Particle Source and Sinks")) {
					ImGui::Checkbox("Enable Source Area", &Settings::ENABLE_SOURCE);

				
					if (!Settings::ENABLE_SOURCE) { ImGui::BeginDisabled(); }

					ImGui::SeparatorText("Source location:");

					ImGui::SliderFloat("Source X:", &sourceA.m_Position.x, -50.0f, 200.0f);
					ImGui::SliderFloat("Source Y:", &sourceA.m_Position.y, -50.0f, 200.0f);

					ImGui::SeparatorText("Generation Config");

					const float   f32_zero = 0.f, f32_three = 3.f;

					ImGui::DragScalar("Generation rate (s)", ImGuiDataType_Float, &sourceA.m_GenerationRate, 0.01f, &f32_zero, &f32_three, "%f", ImGuiSliderFlags_Logarithmic);
					ImGui::SliderFloat("Generation Radius:", &sourceA.m_GenerationRadius, 1.0f, 20.0f);
					
					ImGui::SeparatorText("Starting Velocities");

					ImGui::SliderFloat("X:", &sourceA.m_startingVelocity.x, -200.0f, 200.0f);
					ImGui::SliderFloat("Y:", &sourceA.m_startingVelocity.y, -200.0f, 200.0f);

					if (!Settings::ENABLE_SOURCE) { ImGui::EndDisabled(); }


					ImGui::TreePop();
				}

			}

			if (ImGui::CollapsingHeader("DEBUG MENU")) {

				ImGui::Checkbox("Enable Debug mode", &Settings::ENABLE_DEBUG_MODE);

				static int mode = 0;

				if (!Settings::ENABLE_DEBUG_MODE) { ImGui::BeginDisabled(); }

					if (ImGui::RadioButton("Velocity", mode == Particle::DebugType::D_Velocity)) { mode = Particle::DebugType::D_Velocity; Particle::Debug = Particle::DebugType::D_Velocity; } ImGui::SameLine();
					if (ImGui::RadioButton("Density", mode == Particle::DebugType::D_Density)) { mode = Particle::DebugType::D_Density; Particle::Debug = Particle::DebugType::D_Density; } ImGui::SameLine();
					if (ImGui::RadioButton("Pressure", mode == Particle::DebugType::D_Pressure)) { mode = Particle::DebugType::D_Pressure; Particle::Debug = Particle::DebugType::D_Pressure; }
			
				ImGui::Separator();

				ImGui::Checkbox("Toggle Partitioning Background", &Settings::ENABLE_PARTITION_BACKROUND);
				ImGui::Checkbox("Toggle Particle Info", &Settings::ENABLE_HOVER_PARTICLE);

				if (Settings::ENABLE_HOVER_PARTICLE) {
					ImGuiIO& io = ImGui::GetIO();

					if (ImGui::IsMousePosValid() &&
						io.MousePos.x <= Settings::WINDOW_RESOLUTION.x && io.MousePos.x >= 0 &&
						io.MousePos.y <= Settings::WINDOW_RESOLUTION.y && io.MousePos.y >= 0) {

						// Put in its own function
						float percentageX = io.MousePos.x / Settings::WINDOW_RESOLUTION.x;
						float percentageY = io.MousePos.y / Settings::WINDOW_RESOLUTION.y;

						glm::vec2 normalizedPosition = {
							PhysicsEq::lerp(camera.getProjection().left , camera.getProjection().right, percentageX) - camera.getPosition().x,
							PhysicsEq::lerp(camera.getProjection().top , camera.getProjection().bottom, percentageY) - camera.getPosition().y
						};

						// Could integrate this within code such that there doesnt need to be another for loop.
						for (int i = 0; i < m_ParticleArray->size(); i++) {
							glm::vec4 Particlepos = m_ParticleArray->at(i).m_Position;

							if (normalizedPosition.x <= Particlepos.x + 1 && normalizedPosition.x >= Particlepos.x - 1 &&
								normalizedPosition.y <= Particlepos.y + 1 && normalizedPosition.y >= Particlepos.y - 1) {

								Settings::IS_HOVER_PARTICLE = true;
								Settings::HOVER_PARTICLE = i;

								if (ImGui::IsMouseDown(0)) { Settings::CLICK_PARTICLE = i; ImGui::OpenPopup("Particle Property"); }

								// Stops the loop prematurely, prevents resetting of other particles
								break;
							}
							else {
								Settings::IS_HOVER_PARTICLE = false;
								if (!ImGui::IsPopupOpen("Particle Property")) { Settings::CLICK_PARTICLE = -1; Settings::HOVER_PARTICLE = -1; }
							}
						}
					}
				}
				if (!Settings::ENABLE_DEBUG_MODE) { ImGui::EndDisabled(); }

				if (ImGui::BeginPopup("Particle Property")) {
					Particle particle = m_ParticleArray->at(Settings::CLICK_PARTICLE);

					ImGui::SeparatorText("Particle");

					ImGui::Text("Position: {%.1f,%.1f}", particle.m_Position.x, particle.m_Position.y);
					ImGui::Text("Velocity: {%.1f,%.1f}", particle.getVelocity().x, particle.getVelocity().y);
					ImGui::Text("Acceleration: {%.1f,%.1f}", particle.getAcceleration().x, particle.getAcceleration().y);
					ImGui::Text("Density: %.1f", particle.getDensity());

					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
	}

	inline void T4_Calculate_Density::timeStep() { time += SIMSTEP; }
	
	// Destructor
	T4_Calculate_Density::~T4_Calculate_Density() {
		// Vectors are an object, not an array, and thus don't use []
		delete m_ParticleArray;
		delete m_USP.lookupList;
		delete m_USP.startIndex;
	}
}