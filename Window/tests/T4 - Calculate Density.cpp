#include "T4 - Calculate Density.h"

#include "Camera.h"

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
		m_ClearColour{ 1.0f, 1.0f, 1.0f, 1.0f },
		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f)
	{
		/*
		* 
		* TODO:
		* 
		* This is all done when the object is created.
		* This does not change dynamically, and such all the particle creation
		* needs to be done in its own separate function outside of object creation.
		* 
		* For now, we can leave it, but if we want to dynamically change the no.
		* of particles, then we have to change it.
		* 
		*/
		m_ParticleArray.reserve(MAX_PARTICLES);

		// ------------------------------------------------------------

		float radius = 0.5f;
		float spacing = 0.0f;

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

		// SPH FUNCTIONS HERE
		// Ideally would be in its own class and function.
		Particle::CalculateDensity(m_ParticleArray, m_ParticleArray[0]);


		//float kernel = PhysicsEq::SmoothingKernel(m_ParticleArray[0].m_Position, m_ParticleArray[1].m_Position, 10);
		//std::cout << kernel << std::endl;


		// Updates positions, projections and models.
		m_Proj = glm::ortho(camera.getProjection().left, camera.getProjection().right, camera.getProjection().bottom, camera.getProjection().right, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_MVP = m_Proj * m_View * m_Model;

		// 10k
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			m_ParticleArray[i].update();

			auto collide = Collision::collisionDetection(m_RectContainer, m_ParticleArray[i]);

			if (collide.m_isCollision) {
				Collision::collisionResponse(&m_ParticleArray[i], collide.type);
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
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			CircleBuffer.Draw(m_ParticleArray[i], m_MVP);
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
		ImGui::Text("Draw calls: %i", m_DrawCalls);
		ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);

		ImGui::SliderFloat("Kernel Radius:", &Particle::KERNEL_RADIUS, 0.0f, 5.0f);

		ImGui::Text("Density for particle[0]: %.1f",m_ParticleArray[0].getDensity());
		ImGui::Text("Function value at particle[0]: %.1f", Particle::particleProperties[0]);

	}

	inline void T4_Calculate_Density::timeStep() { time += SIMSTEP; }
	
	/* Shutdown function
	Right now the entire array of particles is stored
					ON THE STACK.
	This should be stored on the heap 'new' command,
	and deleted when necessary. This also allows dynamic
	allocation during runtime.
	
	*/
	void T4_Calculate_Density::Shutdown() {
	
	}
}