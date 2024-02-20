#pragma once
#ifndef PROJECTIMGUI_H
#define PROJECTIMGUI_H

#include "imgui/imgui.h"
#include "Settings.h"



class Gui_Menus {

public:
	static void DisplayMenus();

	// Functions
	static void MainMenu();
	static void SimMenus();
};

#endif 