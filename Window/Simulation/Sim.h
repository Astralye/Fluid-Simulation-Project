#pragma once

class Sim
{
public:
	// Current time of simulation
	float time;

	// The smaller the simstep, the higher the accuracy, but will take longer.
	constexpr static float SIMSTEP = 0.005f; // An integer is one second.

	// 30 FPS = 1/30 = 0.03
	// 60 FPS = 1/60 = 0.016
	// 100 FPS = 1/100 = 0.01
	// 200 FPS = 1/200 = 0.005

	Sim();

	float getTime();
	void timeStep();
};

