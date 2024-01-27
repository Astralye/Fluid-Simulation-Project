// Standard libraries
#include <cmath>
#include <iostream>

// Self made
#include "PhysicsEq.h"

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1.x - c2.x, c1.y - c2.y, c1.y - c2.y);
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
	value = pow(value, 3);

	return value;
}

// f = ma
float PhysicsEq::fmaGetForce(float mass, float acceleration) { return (mass * acceleration); }

// a = f/m
float PhysicsEq::fmaGetAccel(float mass, float force) { return (force / mass); }

// Inputs can be positive or negative, but squaring removes negative values


// Equation for kinetic energy
float PhysicsEq::kineticEnergy(float mass, float velocity) { return (mass * pow(velocity, 2)) / 2; }

// Magnitude for 2D space
float PhysicsEq::pythagoras(float a, float b) { return sqrt(pow(a, 2) + pow(b, 2)); }
// Magnitude for 3D space
float PhysicsEq::pythagoras(float a, float b, float c) { return sqrt(pow(a, 2) + pow(b, 2) + pow(c,2)); }

