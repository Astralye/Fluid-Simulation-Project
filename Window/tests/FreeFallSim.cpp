#include "FreeFallSim.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Simulation/Particle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Sim.h"

#include <chrono>
#include <array>

namespace test {

	FreeFallSim::FreeFallSim()
		: m_Proj(glm::ortho(0.0f, 50.0f, 0.0f, 50.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0))),
		m_ClearColour{ 0.2f, 0.4f, 0.8f, 1.0f },

		m_ParticleA(glm::vec3(22.0f, 10.0f,0.0f)),
		m_ParticleB(glm::vec3(25.0f, 43.0f, 0.0f)),

		m_ParticleC(glm::vec3(19.0f, 45.0f, 0.0f)),
		m_ParticleD(glm::vec3(41.0f, 23.0f, 0.0f)),

		m_ParticleE(glm::vec3(5.0f, 33.0f, 0.0f)),
		m_ParticleF(glm::vec3(26.0f, 46.0f, 0.0f)),

		m_ParticleG(glm::vec3(46.0f, 25.0f, 0.0f)),
		m_ParticleH(glm::vec3(33.0f, 18.0f, 0.0f)),

		m_ParticleI(glm::vec3(20.0f, 24.0f, 0.0f)),
		m_ParticleJ(glm::vec3(11.0f, 51.0f, 0.0f))
	{
		/*
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		*/

		const size_t MaxQuadCount = 50;
		const size_t MaxVertexCount = MaxQuadCount * 4;
		const size_t MaxIndexCount = MaxQuadCount * 6;


		// Create vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Vertex Buffer, This is the layout of all points
		m_VertexBuffer = std::make_unique<VertexBuffer>(MaxQuadCount);
		VertexBufferLayout layout;


		// Layout per positional point
		layout.Push<float>(2); // (X, Y)
		layout.Push<float>(4); // (R, G, B, A)
			//layout.Push<float>(2); // (TexX, TexY)
			//layout.Push<float>(1); // (TexID)


		// Adds the VB to the VA
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		
		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;

		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		// Number of indices in a SINGLE buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);


		m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");

		m_Shader->Bind();
	}

	FreeFallSim::~FreeFallSim()
	{
	}

	void FreeFallSim::OnUpdate(float deltaTime){
		
		float difference = 0;

		auto start = std::chrono::high_resolution_clock::now();
		//while (difference < Sim::SIMSTEP)
		{
			m_ParticleA.update_Accel(0, 0, 0);
			m_ParticleA.update_Vel();
			m_ParticleA.update_Pos(time);


			// Steps
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			difference += duration.count();
		}


		m_ParticleB.update_Accel(0, 0, 0);
		m_ParticleB.update_Vel();
		m_ParticleB.update_Pos(time);
		
		m_ParticleC.update_Accel(0, 0, 0);
		m_ParticleC.update_Vel();
		m_ParticleC.update_Pos(time);

		m_ParticleD.update_Accel(0, 0, 0);
		m_ParticleD.update_Vel();
		m_ParticleD.update_Pos(time);

		m_ParticleE.update_Accel(0, 0, 0);
		m_ParticleE.update_Vel();
		m_ParticleE.update_Pos(time);

		m_ParticleF.update_Accel(0, 0, 0);
		m_ParticleF.update_Vel();
		m_ParticleF.update_Pos(time);

		m_ParticleG.update_Accel(0, 0, 0);
		m_ParticleG.update_Vel();
		m_ParticleG.update_Pos(time);

		m_ParticleH.update_Accel(0, 0, 0);
		m_ParticleH.update_Vel();
		m_ParticleH.update_Pos(time);

		m_ParticleI.update_Accel(0, 0, 0);
		m_ParticleI.update_Vel();
		m_ParticleI.update_Pos(time);

		m_ParticleJ.update_Accel(0, 0, 0);
		m_ParticleJ.update_Vel();
		m_ParticleJ.update_Pos(time);

		timeStep();
	}

	static std::array<Vertex, 4> CreateQuad(Particle p) {

		Vertex v0;
		v0.Position = { p.m_Coords.m_TopLeft.x , p.m_Coords.m_TopLeft.y };
		v0.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v1;
		v1.Position = {p.m_Coords.m_TopRight.x,p.m_Coords.m_TopRight.y};
		v1.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v2;
		v2.Position = { p.m_Coords.m_BottomRight.x, p.m_Coords.m_BottomRight.y };
		v2.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v3;
		v3.Position = { p.m_Coords.m_BottomLeft.x, p.m_Coords.m_BottomLeft.y };
		v3.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		return { v0,v1,v2,v3 };
	}

	void FreeFallSim::OnRender()
	{
		// Set dynamic vertex buffer

		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Refactor this to a for loop

		auto q0 = CreateQuad(m_ParticleA);
		auto q1 = CreateQuad(m_ParticleB);
		auto q2 = CreateQuad(m_ParticleC);
		auto q3 = CreateQuad(m_ParticleD);
		auto q4 = CreateQuad(m_ParticleE);
		auto q5 = CreateQuad(m_ParticleF);
		auto q6 = CreateQuad(m_ParticleG);
		auto q7 = CreateQuad(m_ParticleH);
		auto q8 = CreateQuad(m_ParticleI);
		auto q9 = CreateQuad(m_ParticleJ);

		Vertex vertices[4 * 10];

		memcpy(vertices			  		 , q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + (1 * q0.size()), q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices + (2 * q0.size()), q2.data(), q2.size() * sizeof(Vertex));
		memcpy(vertices + (3 * q0.size()), q3.data(), q3.size() * sizeof(Vertex));
		memcpy(vertices + (4 * q0.size()), q4.data(), q4.size() * sizeof(Vertex));
		memcpy(vertices + (5 * q0.size()), q5.data(), q5.size() * sizeof(Vertex));
		memcpy(vertices + (6 * q0.size()), q6.data(), q6.size() * sizeof(Vertex));
		memcpy(vertices + (7 * q0.size()), q7.data(), q7.size() * sizeof(Vertex));
		memcpy(vertices + (8 * q0.size()), q8.data(), q8.size() * sizeof(Vertex));
		memcpy(vertices + (9 * q0.size()), q9.data(), q9.size() * sizeof(Vertex));

		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		Renderer renderer;

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void FreeFallSim::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);
	}

	inline void FreeFallSim::timeStep()
	{
		time += SIMSTEP;
	}

}