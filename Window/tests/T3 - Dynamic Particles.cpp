#include "T3 - Dynamic Particles.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>
#include <array>
#include <cstdlib>

namespace test {

	T3_Dynamic_Particles::T3_Dynamic_Particles()
		: m_Proj(glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_ClearColour{ 0.2f, 0.4f, 0.8f, 1.0f },
		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f)
	{

		m_ParticleArray.reserve(MAX_PARTICLES);

		int n = 25;
		int mult;

		for (int i = 0; i < MAX_PARTICLES; i++) {
			mult = 1;

			if (i % 2 == 0) {
				mult = -1;
			}

			m_ParticleArray.emplace_back(glm::vec4(10.0f + rand() % 80, 20.0f + rand() % 30, 0.0f, 0.0f), 1.0f, .25f, 1.0f,
				glm::vec3((rand() % n) * mult, (rand() % n), 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f)
			);
		}

		/*
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		*/

		m_QuadBuffer = new Vertex[MaxVertexCount];

		// Create vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Vertex Buffer, This is the layout of all points
		m_VertexBuffer = std::make_unique<VertexBuffer>(VertexType::QUAD,MaxVertexCount);
		VertexBufferLayout layout;

		// Layout per positional point
		layout.Push<float>(2); // (X, Y)
		layout.Push<float>(4); // (R, G, B, A)
		//layout.Push<float>(2); // (TexX, TexY)
		//layout.Push<float>(1); // (TexID)


		// Adds the VB to the VA
		m_VAO->AddBuffer(VertexType::QUAD, *m_VertexBuffer, layout);
		
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

		GLCall(glGenBuffers(1, &QuadIB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
		// Number of indices in a SINGLE buffer READJUST CODE
		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_Shader = std::make_unique<Shader>("res/shaders/quad.shader");
		m_Shader->Bind();
	}

	//Destructor
	T3_Dynamic_Particles::~T3_Dynamic_Particles()
	{
	}

	void T3_Dynamic_Particles::OnUpdate(float deltaTime){
	
		float camZoom = camera.getZoom() / 2;

		m_Proj = glm::ortho(0.0f + camZoom, 100.0f - camZoom, 0.0f + camZoom, 100.0f - camZoom, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));

		// Update Particles movement vectors
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			m_ParticleArray[i].update_Accel();
			m_ParticleArray[i].update_Vel();
			m_ParticleArray[i].update_Pos(time);

			// Check for collision detection for each particle against the container.
			auto collide = Collision::collisionDetection(m_RectContainer, m_ParticleArray[i]);

			if (collide.m_isCollision) {
				Collision::collisionResponse(&m_ParticleArray[i], collide.type);
			}
		}

		//if (Collision::collisionDetection(m_ParticleA, m_ParticleB)) {
		//	Collision::collisionResponse(&m_ParticleA, &m_ParticleB);
		//}

		timeStep();
	}

	void T3_Dynamic_Particles::BeginBatch() {
		m_QuadBufferPtr = m_QuadBuffer;
	}
	
	void T3_Dynamic_Particles::CreateQuad(Particle &p) {

		if (IndexCount >= MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		// Copies data to pointer location
		m_QuadBufferPtr->WorldPosition = { p.m_Coords.m_TopLeft.x , p.m_Coords.m_TopLeft.y, 0.0f, 0.0f };
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = {p.m_Coords.m_TopRight.x,p.m_Coords.m_TopRight.y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = { p.m_Coords.m_BottomRight.x, p.m_Coords.m_BottomRight.y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = { p.m_Coords.m_BottomLeft.x, p.m_Coords.m_BottomLeft.y, 0.0f, 0.0f };
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		IndexCount += 6;
	}

	void T3_Dynamic_Particles::CreateQuad(Rectangle &r) {

		if (IndexCount >= MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		m_QuadBufferPtr->WorldPosition = { r.m_Vertices[0].x , r.m_Vertices[0].y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = { r.m_Vertices[1].x, r.m_Vertices[1].y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = { r.m_Vertices[2].x, r.m_Vertices[2].y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->WorldPosition = { r.m_Vertices[3].x, r.m_Vertices[3].y, 0.0f, 0.0f};
		m_QuadBufferPtr->Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_QuadBufferPtr++;

		IndexCount += 6;
	}

	void T3_Dynamic_Particles::CreateContainer(RectangleContainer &rc) {
		CreateQuad(rc.m_SideA);
		CreateQuad(rc.m_SideB);
		CreateQuad(rc.m_SideC);
		CreateQuad(rc.m_SideD);
	}

	void T3_Dynamic_Particles::EndBatch() {
		GLsizeiptr size = (uint8_t*)m_QuadBufferPtr - (uint8_t*)m_QuadBuffer;
		m_VertexBuffer->Bind();

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_QuadBuffer));
	}

	void T3_Dynamic_Particles::Flush() {

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		m_VAO->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));

		//renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		IndexCount = 0;
	}

	void T3_Dynamic_Particles::OnRender()
	{
		// Set dynamic vertex buffer

		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		BeginBatch();

		// 10 particles = 10*4 = ptr is 40;
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			CreateQuad(m_ParticleArray[i]);
		}

		CreateContainer(m_RectContainer);

		EndBatch();
		Flush();
	}

	void T3_Dynamic_Particles::OnImGuiRender()
	{
		ImGui::Text("Move {W,A,S,D}");
		ImGui::Text("Zoom {-,+}");
		ImGui::Text("Particles: %i", MAX_PARTICLES );

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);
	}

	inline void T3_Dynamic_Particles::timeStep()
	{
		time += SIMSTEP;
	}
	
	void T3_Dynamic_Particles::Shutdown() {
		delete[] m_QuadBuffer;
	}
}