#include "T4 - Calculate Density.h"

#include "Camera.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>
#include <array>
#include <cstdlib>

static const size_t MaxQuadCount = 10000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;

static const int MAX_PARTICLES = 300;

namespace test {

	T4_Calculate_Density::T4_Calculate_Density()
		: m_Proj(glm::ortho(camera.getProjection().left, camera.getProjection().right,
			camera.getProjection().bottom, camera.getProjection().top, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_ClearColour{ 1.0f, 1.0f, 1.0f, 1.0f },
		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f)
	{
		m_ParticleArray.reserve(MAX_PARTICLES);

		// ALGORITHM FOR CREATING EVENLY SPACED SQAURES IN A LARGER SQUARE
		float radius = 2.0f;
		{
			int n = 5;
			int mult;

			int column, row, max_Column, max_Row;
			float spacing = 1.0f;

			float xOffset = m_RectContainer.m_Position.x / 2;
			float yOffset = m_RectContainer.m_Position.y / 2;

			float squareDimention = sqrt(MAX_PARTICLES);

			max_Row = ceil(squareDimention);
			max_Column = floor(squareDimention);

			float xPos, yPos;

			// Set base values
			column = 0;
			row = 0;

			for (int i = 0; i < MAX_PARTICLES; i++) {
				xPos = row * (2 * radius + spacing);
				yPos = column * (2 * radius + spacing);

				row++;

				if (row == max_Row) {
					column++;
					row = 0;
				}
				m_ParticleArray.emplace_back(
					//glm::vec4(xPos + xOffset, yPos + yOffset, 0.0f, 0.0f), 1.0f, radius, 5.0f,
					//glm::vec3(0.0f, 0.0f, 0.0f),

					////Random values, Commented out for Testing purposes.
					glm::vec4(10.0f + rand() % 80, 20.0f + rand() % 30, 0.0f,0.0f), 1.0f, radius, 2.0f,
					glm::vec3((rand() % n), (rand() % n), 0.0f),

					glm::vec3(0.0f, 0.0f, 0.0f)
				);
			}
		}
		
		// Need to optimize better
		float kernel = PhysicsEq::SmoothingKernel(m_ParticleArray[0].m_Position, m_ParticleArray[1].m_Position, 2.0f, m_ParticleArray[0].m_KernelRadius);
		//std::cout << kernel << std::endl;
		
		// Quad Buffer Data
		{
			m_QuadVAO = std::make_unique<VertexArray>();
			m_QuadBuffer = new Vertex[MaxVertexCount];
			m_QuadVertexBuffer = std::make_unique<VertexBuffer>(VertexType::QUAD, MaxVertexCount);
			VertexBufferLayout layout;

			// Layout should be the same with Data_Structure.h
			layout.Push<float>(4); // WorldPosition
			layout.Push<float>(4); // (R, G, B, A)
			m_QuadVAO->AddBuffer(VertexType::QUAD,*m_QuadVertexBuffer, layout);
		}

		uint32_t Quadindices[MaxIndexCount];
		uint32_t offset = 0;

		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			Quadindices[i + 0] = 0 + offset;
			Quadindices[i + 1] = 1 + offset;
			Quadindices[i + 2] = 2 + offset;

			Quadindices[i + 3] = 2 + offset;
			Quadindices[i + 4] = 3 + offset;
			Quadindices[i + 5] = 0 + offset;

			offset += 4;
		}

		// Index Buffer 
		GLCall(glGenBuffers(1, &QuadIB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Quadindices), Quadindices, GL_STATIC_DRAW));

		m_QuadShader = std::make_unique<Shader>("res/shaders/quad.shader");
		m_QuadShader->Bind();

		// New buffer types *HAVE* to be AFTER the index buffers
		// REFACTOR CODE
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// Circle Buffer Data
		{
			m_CircleVAO = std::make_unique<VertexArray>();
			m_CircleBuffer = new CircleVertex[MaxVertexCount];
			m_CircleVertexBuffer = std::make_unique<VertexBuffer>(VertexType::CIRCLE, MaxVertexCount);
			VertexBufferLayout layout;

			// Layout should be the same with Data_Structure.h
			layout.Push<float>(4); // WorldPosition
			layout.Push<float>(4); // LocalPosition
			layout.Push<float>(4); // RGBA
			m_CircleVAO->AddBuffer(VertexType::CIRCLE, *m_CircleVertexBuffer, layout);
		}

		// Circle, Copying for now

		uint32_t Circleindices[MaxIndexCount];
		offset = 0;

		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			Circleindices[i + 0] = 0 + offset;
			Circleindices[i + 1] = 1 + offset;
			Circleindices[i + 2] = 2 + offset;

			Circleindices[i + 3] = 2 + offset;
			Circleindices[i + 4] = 3 + offset;
			Circleindices[i + 5] = 0 + offset;

			offset += 4;
		}

		GLCall(glGenBuffers(1, &CircleIB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CircleIB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Circleindices), Circleindices, GL_STATIC_DRAW));


		// Can be put in their own function and be refactored
		m_CircleShader = std::make_unique<Shader>("res/shaders/Circle.shader");
		m_CircleShader->Bind();
		//m_CircleShader->SetUniform1f("radius", radius);
	}

	//Destructor
	T4_Calculate_Density::~T4_Calculate_Density()
	{
	}

	void T4_Calculate_Density::OnUpdate(float deltaTime){

		// Updates positions, projections and models.
		m_Proj = glm::ortho(camera.getProjection().left, camera.getProjection().right, camera.getProjection().bottom, camera.getProjection().right, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(camera.getPosition().x, camera.getPosition().y, 0));
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_MVP = m_Proj * m_View * m_Model;

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

	// TRY TO REFACTOR BATCH RENDERING INTO ITS OWN FILE. IT TAKE TOO MUCH SPACE HERE
	// MAKES IT HARDER TO READ AND EDIT CODE
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void T4_Calculate_Density::BeginBatch() {

		switch (drawType) {
			case VertexType::QUAD: {
				m_QuadBufferPtr = m_QuadBuffer;
				break;
			}

			case VertexType::CIRCLE: {
 				m_CircleBufferPtr = m_CircleBuffer;
				break;
			}
		}

	}
	
	void T4_Calculate_Density::DrawCircle(Particle &p) {

		if (CircleIndexCount >= MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}

		glm::vec4 vertex;
		for (int i = 0; i < 4; i++) {
			vertex = m_MVP * glm::vec4(p.m_Vertices[i],0.0,1.0);

			m_CircleBufferPtr->WorldPosition = vertex;
			m_CircleBufferPtr->LocalPosition = QuadVertexPositions[i] * 2.0f;
			m_CircleBufferPtr->Colour = { 0.2f, 0.6f, 1.0f, 1.0f };
			m_CircleBufferPtr++;
		}
		CircleIndexCount += 6;
	}

	void T4_Calculate_Density::DrawQuad(Rectangle &r) {

		if (IndexCount >= MaxIndexCount) {
			EndBatch();
			Flush();
			BeginBatch();
		}
		
		for(int i = 0; i < 4; i++){
			glm::vec4 vertex = m_MVP * glm::vec4(r.m_Vertices[i],0.0,1.0);

			m_QuadBufferPtr->WorldPosition = vertex;
			m_QuadBufferPtr->Colour = { 0.2f, 0.2f, 0.2f, 1.0f };
			m_QuadBufferPtr++;
		}
		IndexCount += 6;
	}

	void T4_Calculate_Density::CreateContainer(RectangleContainer &rc) {
		DrawQuad(rc.m_SideA);
		DrawQuad(rc.m_SideB);
		DrawQuad(rc.m_SideC);
		DrawQuad(rc.m_SideD);
	}

	void T4_Calculate_Density::EndBatch() {

		if (drawType == VertexType::QUAD) {
			GLsizeiptr size = (uint8_t*)m_QuadBufferPtr - (uint8_t*)m_QuadBuffer;
			m_QuadVertexBuffer->Bind();

			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_QuadBuffer));
		}

		if(drawType == VertexType::CIRCLE){
			GLsizeiptr size = (uint8_t*)m_CircleBufferPtr - (uint8_t*)m_CircleBuffer;
			m_CircleVertexBuffer->Bind();

			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_CircleBuffer));
		}
	}

	void T4_Calculate_Density::Flush(){

		// We can try use pointers here, but currently I dont know how to do so.
		//Shader *shader;
		switch (drawType) {
			case (VertexType::CIRCLE):
			{
				float value = camera.getProjection().right - camera.getProjection().left;

				m_CircleShader->Bind();
				//m_CircleShader->SetUniform1f("worldCoordinate", value);

				m_CircleVAO->Bind();

				GLCall(glDrawElements(GL_TRIANGLES, CircleIndexCount, GL_UNSIGNED_INT, nullptr));
				CircleIndexCount = 0;

				break;
			}
			case (VertexType::QUAD):
			{
				m_QuadShader->Bind();
				m_QuadVAO->Bind();

				GLCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));
				IndexCount = 0;
				break;
			}
		}
		
		m_DrawCalls++;
	}

	// CODE REFACTOR SHOULD ENDER HERE

	void T4_Calculate_Density::OnRender()
	{
		// Set dynamic vertex buffer
		m_DrawCalls = 0;

		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		drawType = VertexType::QUAD;
		BatchRender(CreateContainer(m_RectContainer));

		drawType = VertexType::CIRCLE;
		BeginBatch();
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			DrawCircle(m_ParticleArray[i]);
		}
		EndBatch();
		Flush();
	}

	void T4_Calculate_Density::OnImGuiRender()
	{
		ImGui::Text("Move {W,A,S,D}");
		ImGui::Text("Zoom {-,+}");
		ImGui::Text("Particles: %i", MAX_PARTICLES);
		ImGui::Text("Draw calls: %i", m_DrawCalls);

		ImGui::Text("Framerate: %.1f FPS", ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);
	}

	inline void T4_Calculate_Density::timeStep()
	{
		time += SIMSTEP;
	}
	
	void T4_Calculate_Density::Shutdown() {
		delete[] m_QuadBuffer;
		delete[] m_CircleBuffer;
	}
}