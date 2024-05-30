// Standard libraries
#include <cmath>
#include <algorithm>
#include <iostream>

// Self made
#include "PhysicsEq.h"


// Static variables
float PhysicsEq::BOUNCE_COEFF = 0.7f;
float PhysicsEq::REST_DENSITY = 1.0f;
float PhysicsEq::STIFFNESS_CONSTANT = 500.0f;
float PhysicsEq::COHESION_COEFF = 1.0f;
float PhysicsEq::EXPONENT = 2.0f;
float PhysicsEq::VISCOSITY = 0.05f;
float PhysicsEq::GRAVITY = -10.0f;

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1-c2);
}

float PhysicsEq::euclid_Squared(glm::vec3 values)
{
	return pow(values.x, 2) + pow(values.y, 2) + pow(values.z, 2);
}

float PhysicsEq::clamp(float value, float min, float max) { return std::max(min, std::min(max, value)); }

// Linear interpolation
float PhysicsEq::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

// Smoothing Kernel Function, W(r - r', h)
float PhysicsEq::SmoothingKernel(const glm::vec3 &positionA, const glm::vec3 &positionB, const float radius)
{
	float distance = euclid_Distance(positionA, positionB);

	if (distance >= radius) { return 0; }
	float volume = (M_PI * pow(radius, 4)) / 6;
	return pow(radius - distance, 2) / volume;
}

// This function is the derivative of the smoothing kernel Equation
// GRAD W(r-r',h)
float PhysicsEq::SmoothingKernelDerivative(float dst, float radius) {
	if (dst > radius) { return 0; }

	float scale = 12 / pow(radius, 4) * M_PI;
	return (dst - radius) * scale;
}

/*
* 
*	Converts density to pressure 
*	====================================================
*
*	Explicit Volume Deviation
* 
*		p		  =	   p_i	 -		 p^0
*	---------------------------------------------
*	Density error	density		target density
* 
* 
*	State Equation SPH (SESPH)
* 
*	From one of many different equations to solve pressure,
*	one example is as follow
* 
*		p	=			k			(	 p_i	-		p^0	)
*	-------------------------------------------------------------
*	pressure	stiffness constant		density		target density
* 
*	
*	SPH simulations that use State equations are compressible.
* 
*	-> More challenging to minimize compression compared to PPE
*	(Pressure Poisson Equation)
*/
float PhysicsEq::ConvertDensityToPressure(float density) {

	//float densityError = density - REST_DENSITY;
	//float pressure = densityError * EXPONENT;
	//return pressure;

	float densityMax = std::max(REST_DENSITY, density);
	return STIFFNESS_CONSTANT * ( pow(densityMax / REST_DENSITY, EXPONENT) - 1.0);

	//float multiplier = (STIFFNESS_CONSTANT * REST_DENSITY) / EXPONENT;
	//float pressureChange = pow((density / REST_DENSITY), EXPONENT) - 1;

	//return multiplier * pressureChange;
}

// Inputs can be positive or negative, but squaring removes negative values

// Magnitude for 2D space
float PhysicsEq::pythagoras(glm::vec2 position) { return (float)sqrt(pow(position.x, 2) + pow(position.y, 2)); }

float PhysicsEq::pythagoras(glm::vec3 position) { return (float)sqrt(pow(position.x, 2) + pow(position.y, 2) + pow(position.z, 2)); }