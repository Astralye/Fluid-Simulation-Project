#include "T4 - Calculate Density.h"

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

static const int MAX_PARTICLES = 10;


namespace test {

	T4_Calculate_Density::T4_Calculate_Density()
		: m_Proj(glm::ortho(0.0f, 50.0f, 0.0f, 50.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_ClearColour{ 0.2f, 0.4f, 0.8f, 1.0f },
		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f)
	{
		m_ParticleArray.reserve(MAX_PARTICLES);

		// ALGORITHM FOR CREATING EVENLY SPACED SQAURES IN A LARGER SQUARE
		{
			int n = 5;
			int mult;

			int column, row, max_Column, max_Row;
			float radius = 10.0f;
			float spacing = 5.0f;

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
					glm::vec3(xPos + xOffset, yPos + yOffset, 0.0f), 1.0f, radius, 10.0f,

					//Random values, Commented out for Testing purposes.
					//glm::vec3(10.0f + rand() % 80, 20.0f + rand() % 30, 0.0f), 1.0f, 2.0f,
					//glm::vec3((rand() % n) * mult, (rand() % n), 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f)
				);
			}
		}

		float kernel = PhysicsEq::SmoothingKernel(m_ParticleArray[0].m_Position, m_ParticleArray[1].m_Position, 2.0f, m_ParticleArray[0].m_KernelRadius);
		//std::cout << kernel << std::endl;

		m_QuadBuffer = new Vertex[MaxVertexCount];

		// Create vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Vertex Buffer, This is the layout of all points
		m_VertexBuffer = std::make_unique<VertexBuffer>(MaxVertexCount);
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

		GLCall(glGenBuffers(1, &QuadIB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
		// Number of indices in a SINGLE buffer READJUST CODE
		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_QuadShader = std::make_unique<Shader>("res/shaders/quad.shader");
		m_QuadShader->Bind();
		m_QuadShader->Unbind();

		m_CircleShader = std::make_unique<Shader>("res/shaders/circle.shader");
		m_CircleShader->Bind();
		m_CircleShader->SetUniform2f("iResolution", 800.0f, 800.0f);
		m_CircleShader->SetUniform1f("radius", 10.0f);
		m_CircleShader->Unbind();

		// Projection size = 100px x 100px
		// Window size = 800px x 800px
		// We need to normalize the values

		// 8 window pixel = 1 Projection

		float window = 800.0f;
		float projection = 800.0f;

		float rad = 10.0f;

		// This value is relative from window to projection matrix
		float ProjToWindow = window / projection;

		// Circle Diameter in terms of Window
		float circleDiameter = 2 * rad * ProjToWindow;
		
		std::cout << "Window pixels to world pixel conversion:" << ProjToWindow << std::endl;

		std::cout << "Circle Diameter in window pixels:" << circleDiameter << std::endl;

		// Scale should change depending on zoom

		//std::cout << scale << std::endl;


		// I need to split the shaders apart, such that I can have multiple shaders
	}

	//Destructor
	T4_Calculate_Density::~T4_Calculate_Density()
	{
	}

	void T4_Calculate_Density::OnUpdate(float deltaTime){
	
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

	void T4_Calculate_Density::BeginBatch() {
		m_QuadBufferPtr = m_QuadBuffer;
	}
	
	void T4_Calculate_Density::CreateQuad(Particle &p) {

		if (IndexCount >= MaxIndexCount) {
			EndBatch();
			Flush(CircleShader);
			BeginBatch();
		}

		// Copies data to pointer location
		m_QuadBufferPtr->Position = { p.m_Coords.m_TopLeft.x , p.m_Coords.m_TopLeft.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = {p.m_Coords.m_TopRight.x,p.m_Coords.m_TopRight.y};
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = { p.m_Coords.m_BottomRight.x, p.m_Coords.m_BottomRight.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = { p.m_Coords.m_BottomLeft.x, p.m_Coords.m_BottomLeft.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		IndexCount += 6;
	}

	void T4_Calculate_Density::CreateQuad(Rectangle &r) {

		if (IndexCount >= MaxIndexCount) {
			EndBatch();
			Flush(QuadShader);
			BeginBatch();
		}

		m_QuadBufferPtr->Position = { r.m_Coords.m_TopLeft.x , r.m_Coords.m_TopLeft.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = { r.m_Coords.m_TopRight.x, r.m_Coords.m_TopRight.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = { r.m_Coords.m_BottomRight.x, r.m_Coords.m_BottomRight.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		m_QuadBufferPtr->Position = { r.m_Coords.m_BottomLeft.x, r.m_Coords.m_BottomLeft.y };
		m_QuadBufferPtr->Colour = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_QuadBufferPtr++;

		IndexCount += 6;
	}

	void T4_Calculate_Density::CreateContainer(RectangleContainer &rc) {
		CreateQuad(rc.m_SideA);
		CreateQuad(rc.m_SideB);
		CreateQuad(rc.m_SideC);
		CreateQuad(rc.m_SideD);
	}

	void T4_Calculate_Density::EndBatch() {
		GLsizeiptr size = (uint8_t*)m_QuadBufferPtr - (uint8_t*)m_QuadBuffer;
		m_VertexBuffer->Bind();

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_QuadBuffer));
	}

	void T4_Calculate_Density::Flush(ShaderProgram shaderProg) {

		Renderer renderer;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;

		// Here we bind the shader, this is independent of the actual shader code itself.
	
		// We can try use pointers here, but currently I dont know how to do so.
		//Shader *shader;
		switch (shaderProg) {
			case (CircleShader):
			{
				m_CircleShader->Bind();
				m_CircleShader->SetUniformMat4f("u_MVP", mvp);
				break;
			}
			case (QuadShader):
			{
				m_QuadShader->Bind();
				m_QuadShader->SetUniformMat4f("u_MVP", mvp);
				break;
			}
		}

		m_VAO->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));

		m_DrawCalls++;
		IndexCount = 0;
	}

	void T4_Calculate_Density::OnRender()
	{
		// Set dynamic vertex buffer
		m_DrawCalls = 0;

		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// All particles are under their own batch.
		BeginBatch();
		for (int i = 0; i < m_ParticleArray.size(); i++) {
			CreateQuad(m_ParticleArray[i]);
		}
		Flush(CircleShader);
		EndBatch();

		//BeginBatch();
		//CreateContainer(m_RectContainer);
		//Flush(QuadShader);
		//EndBatch();
	}

	void T4_Calculate_Density::OnImGuiRender()
	{
		ImGui::Text("Move {W,A,S,D}");
		ImGui::Text("Zoom {-,+}");
		ImGui::Text("Particles: %i", MAX_PARTICLES);
		ImGui::Text("Draw calls: %i", m_DrawCalls);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);
	}

	inline void T4_Calculate_Density::timeStep()
	{
		time += SIMSTEP;
	}

	void T4_Calculate_Density::movementData(Camera cam) {

		float camZoom = cam.zoom / 2;

		m_Proj = glm::ortho(0.0f + camZoom, 800.0f - camZoom, 0.0f + camZoom, 800.0f - camZoom, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(cam.position.x, cam.position.y, 0));
	}
	
	void T4_Calculate_Density::Shutdown() {
		delete[] m_QuadBuffer;
	}
}