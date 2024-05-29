#pragma once
#ifndef SPH_H
#define SPH_H

#include <vector>
#include "Simulation/Particle.h"

class SPH {
public:

	static void CalculateAllDensities(std::vector<Particle>* particleArray, std::vector<int> &particlesMaincell, std::vector<int> &particleIndices);
	static void CalculateAllDensities(std::vector<Particle>* particleArray);

	static void CalculateDensity(std::vector<Particle>* arr, std::vector<int>& particleIndices, int j);
	static void CalculateDensity(std::vector<Particle>* arr, int j);

	static void CalculatePositionCollision(std::vector<Particle>* arr, std::vector<int>& particlesMaincell, RectangleContainer& container, bool isBorder);
	static void CalculatePositionCollision(std::vector<Particle>* arr, RectangleContainer& container);

	static void CalculateAllPressures(std::vector<Particle>* particleArray, std::vector<int>& particlesMaincell, std::vector<int>& particleIndices);
	static void CalculateAllPressures(std::vector<Particle>* particleArray);

	static glm::vec2 CalculatePressureForce(std::vector<Particle>* arr, std::vector<int>& particlesMaincell, int j);
	static glm::vec2 CalculatePressureForce(std::vector<Particle>* arr, int j);

	static void CalculateAllViscosities(std::vector<Particle>* particleArray, std::vector<int>& particlesMaincell, std::vector<int>& particleIndices);
	static void CalculateAllViscosities(std::vector<Particle>* particleArray);

	static void CalculateViscosity(std::vector<Particle>* particleArray, int j);
	static void CalculateViscosity(std::vector<Particle>* particleArray, std::vector<int>& particleIndices, int j);

	static glm::vec2 CalculateAdhesion(std::vector<Particle>* particleArray, std::vector<int>& particlesMaincell, int j);
};

#endif
