// Standard libraries
#include <cmath>
#include <algorithm>

// Self made
#include "PhysicsEq.h"

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1.x - c2.x, c1.y - c2.y, c1.y - c2.y);
}


float PhysicsEq::clamp(float value, float min, float max) { return std::max(min, std::min(max, value)); }

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

