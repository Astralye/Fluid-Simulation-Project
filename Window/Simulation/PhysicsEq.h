#ifndef PHYSICS_EQUATIONS_H
#define PHYSICS_EQUATIONS_H

#pragma once
#include "glm/glm.hpp"

class PhysicsEq
{

// Physics equations are public. Can be used anywhere
public:

	constexpr static float GRAVITY = -10 ; // This is in m/s

	static float kineticEnergy(float mass, float velocity);

	// Pythagorean theorem
	static float pythagoras( float a, float b );
	static float pythagoras( float a, float b, float c );

	// Euclidean distance
	static float euclid_Distance(glm::vec3 c1, glm::vec3 c2);

	static float fmaGetForce(float mass, float acceleration);
	static float fmaGetAccel(float mass, float force);

	static float clamp(float value, float min, float max);

	static float calcDensity();

	static float SmoothingKernel(const glm::vec3 &particleA, const glm::vec3 &particleB, float height, float radius);

	// Always returns Negative value
	static inline float toNegative(float value) { return -toPositive(value); }

	// Always returns positive value
	static inline float toPositive(float value) { return abs(value); }
};

#endif