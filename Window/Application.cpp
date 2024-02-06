
//#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "Camera.h"
#include "Settings.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include "tests/Test.h"
#include "tests/T4 - Calculate Density.h"
#include "tests/T4 - Calculate Density.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void){

// GLFW Functions, OpenGL Window functions
// Start --------------------------------------------------------------------------
    
    if (!glfwInit()) // Initialize GLFW
        return -1; 

	// MAKE SURE TO CHANGE IMGUI TO LATEST FILES.
	// DO THIS AFTER I HAVE FIXED ALL THE BUGS
	// DO NOT FORGET
	// 
	// GL 4.6 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // GLFW window
	GLFWwindow* window = glfwCreateWindow((int)WINDOW_RESOLUTION.x, (int)WINDOW_RESOLUTION.y, "Fluid Simulation", nullptr, nullptr);
    if (!window){
        glfwTerminate();
        return -1;
    }

	// Key input
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

// End --------------------------------------------------------------------------

    // GLEW, Loads OpenGL functions has to be initialized AFTER creation of window
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    // OpenGL Version
    std::cout << glGetString(GL_VERSION) << std::endl;
    
	Renderer renderer;

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Pointer to the current test object
	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;
	testMenu->RegisterTest<test::T4_Calculate_Density>("T4 - Calculate Density");

	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		renderer.Clear();

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// If currentTest exists
		if (currentTest) {

			currentTest->OnUpdate(SIMSTEP);
			currentTest->OnRender();
			ImGui::Begin("Test");

			// If the test is the not test menu
			if (currentTest != testMenu)
			{

				if (ImGui::Button("<-")) {
					// Runs only if button click, delete the current test
					delete currentTest;
					currentTest = testMenu;
				}
			}

			currentTest->OnImGuiRender();
			ImGui::End();
		}


		// Render GUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	currentTest->Shutdown();

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Need a way of only running a certain code when within a function loop.

	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		switch (key) {
		case GLFW_KEY_A:
			camera.move(camera.x, true);
			break;

		case GLFW_KEY_D:
			camera.move(camera.x, false);
			break;

		case GLFW_KEY_W:
			camera.move(camera.y, false);
			break;

		case GLFW_KEY_S:
			camera.move(camera.y, true);
			break;

		case GLFW_KEY_MINUS:
			camera.zoom(false);
			break;

		case GLFW_KEY_EQUAL:
			camera.zoom(true);
			break;
		}
	}

}