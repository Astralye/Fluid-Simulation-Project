#pragma once
#ifndef SPH_H
#define SPH_H

#include <vector>
#include "Simulation/Particle.h"

class SPH {
public:

	static void CalculateAllDensities(std::vector<Particle>* particleArray);
	static void CalculateDensity(std::vector<Particle>* arr, int j);
	static void CalculatePositionCollision(std::vector<Particle>* arr, RectangleContainer& container);

	static void CalculateAllPressures(std::vector<Particle>* particleArray);
	static glm::vec2 CalculatePressureForce(std::vector<Particle>* arr, int j);

	static void CalculateAllViscosities(std::vector<Particle>* particleArray);
	static void CalculateViscosity(std::vector<Particle>* particleArray, int j);
};

#endif
