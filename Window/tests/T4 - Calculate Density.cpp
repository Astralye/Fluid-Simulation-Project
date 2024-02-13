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

namespace test {

	T4_Calculate_Density::T4_Calculate_Density()
		: m_Proj(glm::ortho(camera.getProjection().left, camera.getProjection().right,
			camera.getProjection().bottom, camera.getProjection().top, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_MVP(m_Proj * m_View * m_Model),

		m_ClearColour{ 1.0f, 1.0f, 1.0f, 1.0f },
		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f),
		drawType(VertexType::Null),
		m_DrawCalls(0),
		time(0)
	{

		// TODO
		// ISSUE : m_ParticleArray uses 'new' and 'reserve'
		// 
		// MAX_PARTICLES cannot be less than 40
		// For some reason there is a memory leak or something

		// Array is created on the heap.
		m_ParticleArray = new std::vector<Particle>;
		m_ParticleArray->reserve(MAX_PARTICLES);

		// ------------------------------------------------------------

		float radius = 1.0f;
		float spacing = 0.75f;

		Particle::init_Cube(m_ParticleArray,radius,spacing);
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

		// Updates positions, projections and models.
		m_Proj = glm::ortho(camera.getProjection().left, camera.getProjection().right, camera.getProjection().bottom, camera.getProjection().right, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_MVP = m_Proj * m_View * m_Model;


		// SPH FUNCTIONS HERE
		// Ideally would be in its own class and function.

		// Updates all density values
		for (int i = 0; i < m_ParticleArray->size(); i++) {
			Particle::CalculateDensity(m_ParticleArray, m_ParticleArray->at(i),i);
		}

		// Updates all pressure values
		for (int i = 0; i < m_ParticleArray->size(); i++) {
			glm::vec2 pressureForce = Particle::CalculatePressureForce(m_ParticleArray, m_ParticleArray->at(i),i);

			// F = MA --> A = F / M
			if (m_ParticleArray->at(i).getDensity() != 0) {
				glm::vec2 pressureAcceleration = pressureForce / m_ParticleArray->at(i).getDensity();
				m_ParticleArray->at(i).setAcceleration(pressureAcceleration);
			}

			if (i == 48) {
				m_testvalue = pressureForce;
			}
		}
		
		// 10k
		for (int i = 0; i < m_ParticleArray->size(); i++) {

			m_ParticleArray->at(i).update();

			// Check for collision detection for each particle against the container.
			auto collide = Collision::collisionDetection(m_RectContainer, m_ParticleArray->at(i));

			if (collide.m_isCollision) {
				Collision::collisionResponse(m_ParticleArray->at(i), collide.type);
			}
		}

		// Collision between particles
		// Omitted this as it isn't really relevant
		//if (Collision::collisionDetection(m_ParticleA, m_ParticleB)) {
		//	Collision::collisionResponse(&m_ParticleA, &m_ParticleB);
		//}

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
	void T4_Calculate_Density::OnImGuiRender()
	{
		ImGui::Text("Move {W,A,S,D}");
		ImGui::Text("Zoom {-,+}");
		ImGui::Text("Particles: %i", MAX_PARTICLES);
		//ImGui::Text("Draw calls: %i", m_DrawCalls);

		ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);

		ImGui::Text("Density of particle[0]: %.3f", m_ParticleArray->at(0).getDensity());
		//ImGui::Text("Kernel result of particle[0]: %.3f", m_testvalue);

		ImGui::SliderFloat("Pressure Multiplier:", &PhysicsEq::STIFFNESS_CONSTANT, 0.0f, 1.0f);
		ImGui::SliderFloat("Target Density:", &PhysicsEq::targetDensity, 0.7f, 1.5f);

		ImGui::SliderFloat("Kernel Radius:", &Particle::KERNEL_RADIUS, 0.0f, 5.0f);



		ImGui::SliderFloat("Particle[48].x :", &m_ParticleArray->at(48).m_Position.x, 20.0f, 40.0f);
		ImGui::SliderFloat("Particle[48].y :", &m_ParticleArray->at(48).m_Position.y, 20.0f, 40.0f);


		ImGui::Text("Particle[48] pressure: { %.1f, %.1f } ", m_testvalue.x, m_testvalue.y);

	}

	inline void T4_Calculate_Density::timeStep() { time += SIMSTEP; }
	
	void T4_Calculate_Density::Shutdown() {
		delete[] m_ParticleArray;
	}
}