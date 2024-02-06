// Standard libraries
#include <cmath>
#include <algorithm>
#include <iostream>

// Self made
#include "PhysicsEq.h"

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1.x - c2.x, c1.y - c2.y, c1.z - c2.z);
}


float PhysicsEq::clamp(float value, float min, float max) { return std::max(min, std::min(max, value)); }

float PhysicsEq::calcDensity()
{
	return 0.0f;
}

// Possibly refactor code.
float PhysicsEq::SmoothingKernel(const glm::vec3 &positionA, const glm::vec3 &positionB, float height, float radius)
{
	// Finds the length between two points within a 3 dimensional space.
	float distance = euclid_Distance(positionA, positionB);

	// This is currently a linear function
	float value = std::max(0.0f, radius - distance);
	
	// Cubic function
	value = (float)pow(value, 3);

	return value;
}

// f = ma
float PhysicsEq::fmaGetForce(float mass, float acceleration) { return (mass * acceleration); }

// Smoothing Kernel Function, W(r - r', h)
float PhysicsEq::SmoothingKernel(const glm::vec3 &positionA, const glm::vec3 &positionB, const float radius)
{

	// Finds the length between two points within a 3 dimensional space.
	float distance = euclid_Distance(positionA, positionB);

	// This is currently a linear function
	float value = std::max(0.0f, float( pow(radius,2) - pow(distance,2) ));
	
	// Cubic function
	value = (float)pow(value, 3);

	return value;
}

// This function is the derivative of the smoothing kernel Equation
float PhysicsEq::SmoothingKernelDerivative(float dst, float radius) {
	if (dst > radius) { return 0; }

	float f = pow(radius,2) - pow(dst,2);
	float scale = -24 / (M_PI * pow(radius, 8));
	return scale * dst * pow(f,2);
}


// Inputs can be positive or negative, but squaring removes negative values

// Magnitude for 2D space
float PhysicsEq::pythagoras(float a, float b) { return (float)sqrt(pow(a, 2) + pow(b, 2)); }
// Magnitude for 3D space
float PhysicsEq::pythagoras(float a, float b, float c) { return (float)sqrt(pow(a, 2) + pow(b, 2) + pow(c,2)); }

