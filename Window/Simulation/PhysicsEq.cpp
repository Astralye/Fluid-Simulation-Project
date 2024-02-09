// Standard libraries
#include <cmath>
#include <algorithm>
#include <iostream>

// Self made
#include "PhysicsEq.h"


// Static variables
float PhysicsEq::targetDensity = 0.0f;
float PhysicsEq::pressureMultiplier = 0.0f;

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1.x - c2.x, c1.y - c2.y);
}


float PhysicsEq::clamp(float value, float min, float max) { return std::max(min, std::min(max, value)); }

// Smoothing Kernel Function, W(r - r', h)
float PhysicsEq::SmoothingKernel(const glm::vec3 &positionA, const glm::vec3 &positionB, const float radius)
{

	float distance = euclid_Distance(positionA, positionB);

	float q = ( 1 / radius ) * distance;

	float result;

	if (0 <= q && q <= 0.5) {
		return (6 * (pow(q, 3) - pow(q, 2))) + 1;
	}
	else if (0.5 < q && q <= 1) {
		return 2 * pow((1 - q), 3);
	}
	else {
		return 0;
	}
	
	//// Finds the length between two points within a 3 dimensional space.
	//float distance = euclid_Distance(positionA, positionB);

	//// This is currently a linear function
	//float value = std::max(0.0f, float( pow(radius,2) - pow(distance,2) ));
	//
	//// Cubic function
	//value = (float)pow(value, 3);

	//return value;
}

// This function is the derivative of the smoothing kernel Equation
float PhysicsEq::SmoothingKernelDerivative(float dst, float radius) {
	if (dst > radius) { return 0; }

	float f = pow(radius,2) - pow(dst,2);
	float scale = -24 / (M_PI * pow(radius, 8));
	return scale * dst * pow(f,2);
}

float PhysicsEq::ConvertDensityToPressure(float density) {
	float densityError = density - targetDensity;
	float pressure = densityError * pressureMultiplier;
	return pressure;
}

// Inputs can be positive or negative, but squaring removes negative values

// Magnitude for 2D space
float PhysicsEq::pythagoras(float a, float b) { return (float)sqrt(pow(a, 2) + pow(b, 2)); }
// Magnitude for 3D space
float PhysicsEq::pythagoras(float a, float b, float c) { return (float) sqrt(pow(a, 2) + pow(b, 2) + pow(c,2)); }

