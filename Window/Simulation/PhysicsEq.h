#pragma once

#ifndef PHYSICS_EQUATIONS_H
#define PHYSICS_EQUATIONS_H

#define _USE_MATH_DEFINES
#include <math.h>

#include "glm/glm.hpp"

class PhysicsEq
{

// Physics equations are public. Can be used anywhere
public:

	static float BOUNCE_COEFF;
	static float REST_DENSITY;
	static float STIFFNESS_CONSTANT;
	static float COHESION_COEFF;
	static float EXPONENT;
	static float VISCOSITY;
	static float GRAVITY; // This is in m/s

	// Pythagorean theorem
	static float pythagoras(glm::vec2 position);
	static float pythagoras(glm::vec3 position);

	// Linear Interpolation
	static float lerp(float a, float b, float f);

	// Euclidean distance
	static float euclid_Distance(glm::vec3 c1, glm::vec3 c2);
	static float euclid_Squared(glm::vec3 values);

	static float clamp(float value, float min, float max);

	static float SmoothingKernel(const glm::vec3& particleA, const glm::vec3& particleB, const float radius);

	static float SmoothingKernelDerivative(float dst, float radius);

	static float ConvertDensityToPressure(float density);

	// Always returns Negative value
	static inline float toNegative(float value) { return -toPositive(value); }

	// Always returns positive value
	static inline float toPositive(float value) { return abs(value); }
};

#endif