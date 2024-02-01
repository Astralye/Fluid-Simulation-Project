#include "T2 - Collision.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <chrono>
#include <array>

namespace test {

	T2_Collision::T2_Collision()
		: m_Proj(glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_ClearColour{ 0.2f, 0.4f, 0.8f, 1.0f },

		m_ParticleA(glm::vec4( 50.0f, 30.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f,
					glm::vec3( 20.0f, 15.0f, 0.0f),
					glm::vec3(  0.0f,  0.0f, 0.0f)),

		m_ParticleB(glm::vec4( 30.0f, 20.0f, 0.0f, 0.0f), 1.0f, 5.0f, 1.0f,
					glm::vec3(  2.0f,  5.0f, 0.0f),
					glm::vec3(  0.0f,  0.0f, 0.0f)),

		m_RectContainer(glm::vec3(50.0f, 50.0f, 0.0f), 90.0f, 80.0f)

		//m_ParticleC(glm::vec3(70.0f, 130.0f, 0.0f), 2.0f, 10.0f,
		//	glm::vec3(60.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f))

	{
		/*
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		*/

		const size_t MaxQuadCount = 100;
		const size_t MaxVertexCount = MaxQuadCount * 4;
		const size_t MaxIndexCount = MaxQuadCount * 6;

		// Create vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Vertex Buffer, This is the layout of all points
		m_VertexBuffer = std::make_unique<VertexBuffer>(VertexType::QUAD,MaxQuadCount);
		VertexBufferLayout layout;
		// [ X, Y , R , G , B ,A ]

		// Layout per positional point
		layout.Push<float>(2); // (X, Y)
		layout.Push<float>(4); // (R, G, B, A)
		//layout.Push<float>(2); // (TexX, TexY)
		//layout.Push<float>(1); // (TexID)


		// Adds the VB to the VA
		m_VAO->AddBuffer(VertexType::QUAD,*m_VertexBuffer, layout);
		
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

	//Destructor
	T2_Collision::~T2_Collision()
	{
	}

	void T2_Collision::OnUpdate(float deltaTime){
		
		//std::cout << "A,Velocity = " << m_ParticleA.getVelocity().x << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		//while (difference < Sim::SIMSTEP)
		{
			m_ParticleA.update_Accel();
			m_ParticleA.update_Vel();
			m_ParticleA.update_Pos(time);


			// Steps
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			//difference += duration.count();
		}


		m_ParticleB.update_Accel();
		m_ParticleB.update_Vel();
		m_ParticleB.update_Pos(time);


		//m_ParticleC.update_Accel(0, 0, 0);
		//m_ParticleC.update_Vel();
		//m_ParticleC.update_Pos(time);

		//if (Collision::collisionDetection(m_ParticleA, m_ParticleB)) {
		//	Collision::collisionResponse(&m_ParticleA, &m_ParticleB);
		//}

		auto collide = Collision::collisionDetection(m_RectContainer, m_ParticleA);
		auto collide2 = Collision::collisionDetection(m_RectContainer, m_ParticleB);

		if (collide.m_isCollision) {
			Collision::collisionResponse(&m_ParticleA,collide.type);
		}

		if (collide2.m_isCollision) {
			Collision::collisionResponse(&m_ParticleB, collide2.type);
		}


		timeStep();
	}

	static std::array<Vertex, 4> CreateQuad(Particle &p) {

		Vertex v0;
		v0.WorldPosition = { p.m_Coords.m_TopLeft.x , p.m_Coords.m_TopLeft.y,0.0f,0.0f };
		v0.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v1;
		v1.WorldPosition = {p.m_Coords.m_TopRight.x,p.m_Coords.m_TopRight.y,0.0f,0.0f };
		v1.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v2;
		v2.WorldPosition = { p.m_Coords.m_BottomRight.x, p.m_Coords.m_BottomRight.y,0.0f,0.0f };
		v2.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v3;
		v3.WorldPosition = { p.m_Coords.m_BottomLeft.x, p.m_Coords.m_BottomLeft.y ,0.0f,0.0f };
		v3.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		return { v0,v1,v2,v3 };
	}

	static std::array<Vertex, 4> CreateQuad(Rectangle &r) {

		Vertex v0;
		v0.WorldPosition = { r.m_Vertices[0].x , r.m_Vertices[0].y, 0.0f,0.0f};
		v0.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v1;
		v1.WorldPosition = { r.m_Vertices[1].x, r.m_Vertices[1].y, 0.0f,0.0f};
		v1.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v2;
		v2.WorldPosition = { r.m_Vertices[2].x, r.m_Vertices[2].y, 0.0f,0.0f};
		v2.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		Vertex v3;
		v3.WorldPosition = { r.m_Vertices[3].x, r.m_Vertices[3].y, 0.0f,0.0f};
		v3.Colour = { 0.5f, 0.5f, 0.5f, 1.0f };

		return { v0,v1,v2,v3 };
	}

	static std::array<Vertex, 16> CreateContainer(RectangleContainer &rc) {

		auto a = CreateQuad(rc.m_SideA);
		auto b = CreateQuad(rc.m_SideB);
		auto c = CreateQuad(rc.m_SideC);
		auto d = CreateQuad(rc.m_SideD);

		return { a[0],a[1],a[2],a[3],
				b[0],b[1],b[2],b[3],
				c[0],c[1],c[2],c[3],
				d[0],d[1],d[2],d[3]
		};
	}

	void T2_Collision::OnRender()
	{
		// Set dynamic vertex buffer

		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Refactor this to a for loop

		auto q0 = CreateQuad(m_ParticleA);
		auto q1 = CreateQuad(m_ParticleB);
		//auto q2 = CreateQuad(m_ParticleC);

		// Rectangle -> 1. 4 Rect -> 4

		auto rc1 = CreateContainer(m_RectContainer);

		Vertex vertices[4 * 6];

		memcpy(vertices			  		 , q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + (1 * q0.size()), q1.data(), q1.size() * sizeof(Vertex));
		//memcpy(vertices + (2 * q0.size()), q2.data(), q2.size() * sizeof(Vertex));

		memcpy(vertices + (2 * q0.size()), rc1.data(), rc1.size() * sizeof(Vertex));

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

	void T2_Collision::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Time: %.3f", time);
	}

	inline void T2_Collision::timeStep()
	{
		time += SIMSTEP;
	}

}