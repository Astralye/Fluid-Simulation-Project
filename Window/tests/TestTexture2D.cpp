#include "TestTexture2D.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{

		float positions[] = {

			//  2, 4, 1
		//	 {  X   |   Y  } {  R   |   G  |  B  |  A  }{TexX|TexY} {TexID}
			  50.0f,   50.0f, 0.18f,  0.6f, 0.96f, 1.0f, 
			 100.0f,   50.0f, 0.18f,  0.6f, 0.96f, 1.0f, 
			 100.0f,  100.0f, 0.18f,  0.6f, 0.96f, 1.0f, 
			  50.0f,  100.0f, 0.18f,  0.6f, 0.96f, 1.0f, 

			 400.0f,  400.0f,  1.0f, 0.93f, 0.24f, 1.0f, 
			 500.0f,  400.0f,  1.0f, 0.93f, 0.24f, 1.0f,
			 500.0f,  500.0f,  1.0f, 0.93f, 0.24f, 1.0f,
			 400.0f,  500.0f,  1.0f, 0.93f, 0.24f, 1.0f, 
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Vertex Buffer, This is the layout of all points
		m_VertexBuffer = std::make_unique<VertexBuffer>(6 * 4 * 2);
		VertexBufferLayout layout;

		// Layout per positional point
		layout.Push<float>(2); // (X, Y)
		layout.Push<float>(4); // (R, G, B, A)
		//layout.Push<float>(2); // (TexX, TexY)
		//layout.Push<float>(1); // (TexID)

		// Adds the VB to the VA
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		
		// Number of indices in a SINGLE buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

		m_Shader = std::make_unique<Shader>("res/shaders/BasicShader.h");
		m_Shader->Bind();


		m_Texture = std::make_unique<Texture>("res/textures/Buh.png");
		/*
		m_TextureB = std::make_unique<Texture>("res/textures/xdd.png");

		m_TextureA->Bind(0);
		m_TextureB->Bind(1);
		*/

		//m_Shader->SetUniform1i("u_Texture", 0);

	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.2f,0.4f,0.6f,1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		//m_Texture->Bind();
		
		Renderer renderer;

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		// This is rendering the same object but different location.!!!!
		/*
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		*/

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 720.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 720.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
