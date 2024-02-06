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

	constexpr static float GRAVITY = -10 ; // This is in m/s

	// Pythagorean theorem
	static float pythagoras( float a, float b );
	static float pythagoras( float a, float b, float c );

	// Euclidean distance
	static float euclid_Distance(glm::vec3 c1, glm::vec3 c2);

	static float clamp(float value, float min, float max);

	static float SmoothingKernel(const glm::vec3& particleA, const glm::vec3& particleB, const float radius);

	static float SmoothingKernelDerivative(float dst, float radius);

	// Always returns Negative value
	static inline float toNegative(float value) { return -toPositive(value); }

	// Always returns positive value
	static inline float toPositive(float value) { return abs(value); }
};

#endif