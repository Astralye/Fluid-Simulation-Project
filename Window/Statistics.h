#pragma once

#ifndef STATISTICS_H
#define STATISTICS_H

#include "Simulation/Particle.h"
#include "tests/Test.h"
 
#include <chrono>
#include <vector>

struct Timer {

	std::chrono::time_point<std::chrono::system_clock> start, end;

	void startTimer();
	void endTimer(std::chrono::duration<float>& timer);

	float a;
};

struct Statistics {
	std::chrono::duration<float> Time_Create_Lookup;
	std::chrono::duration<float> Time_Sort_Lookup;
	std::chrono::duration<float> Time_Neighbour_Cells;



	//std::chrono::duration<float> Time_Calculate_Density;
	//std::chrono::duration<float> Time_Calculate_Pressure;
	//std::chrono::duration<float> Time_Calculate_Movement;
	//std::chrono::duration<float> Time_Calculate_Viscosity;
	std::chrono::duration<float> Time_Render_Particles;

	int m_DrawCalls;
};

void TIME(void (test::Test::*func)(), test::Test* a, std::chrono::duration<float>& timer);

void TIME(void (*func)(std::vector<Particle>*),
	std::vector<Particle>* particleArray, std::chrono::duration<float>& timer);

void TIME(void (*func)(std::vector<Particle>*, RectangleContainer& container),
	std::vector<Particle>* particleArray, RectangleContainer& container, std::chrono::duration<float>& timer);

extern Statistics stats;
extern Timer timer;

#endif 
