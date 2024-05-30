#include "Project_ImGui.h"
#include "imgui/imgui.h"

#include <windows.h>
#include <shellapi.h>


void Gui_Menus::DisplayMenus() {
	MainMenu();
}



// This will be displayed on EVERY page
// This is similar to any editing application
// These functions should be global
// Easily navigatible
void Gui_Menus::MainMenu() {

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Application")) {

			if (ImGui::MenuItem("Settings")) {
				ImGui::OpenPopup("settings_popup");

				if (ImGui::BeginPopup("settings_popup", ImGuiWindowFlags_MenuBar)) {

					ImGui::Text("Settings popup");

					ImGui::EndPopup();
				}
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Back To Menu")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Simulation")) {
			if (ImGui::MenuItem("Start")) {}
			if (ImGui::MenuItem("Stop")) {}
			if (ImGui::MenuItem("Restart")) {}

			ImGui::Separator();

			if (ImGui::MenuItem("Change Simulation")) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug")) {
			if (ImGui::MenuItem("Start")) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Github")) {
				ShellExecute(0, 0, L"https://github.com/Astralye/Fluid-Simulation-Project", 0, 0, SW_SHOW);
			
			}

			//ImGui::OpenPopup("Help_Popup");

			//if (ImGui::BeginPopup("Help_Popup"))

			//if (ImGui::MenuItem("Controls")) {

			//	// When user clicks 'Help'
			//	// Creates a popup
			//	// Pop up contains tabs for the user to navigate
			//	// What help they need
			//	
			//	/*
			//	Help -> {
			//		Controls
			//		Menus
			//		Simulation
			//		??
			//	}
			//	
			//	
			//	*/


			//	ImGui::Begin("Controls");
			//	{
			//		ImGui::Text("Move {W,A,S,D}");
			//		ImGui::Text("Zoom {-,+}");
			//	}
			//	ImGui::End();
			//}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

}

void Gui_Menus::SimMenus(){
}