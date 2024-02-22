// Standard libraries
#include <cmath>
#include <algorithm>
#include <iostream>

// Self made
#include "PhysicsEq.h"


// Static variables
float PhysicsEq::REST_DENSITY = 3.0f;
float PhysicsEq::STIFFNESS_CONSTANT = 43000.0f;
float PhysicsEq::EXPONENT = 4.4f;
float PhysicsEq::VISCOSITY = 0.049f;
float PhysicsEq::GRAVITY = -100.0f;

// Euclidean distance is the length between points on an axis.
float PhysicsEq::euclid_Distance(glm::vec3 c1, glm::vec3 c2)
{
	// Returns a non-negative value as results of pythagoras can only be non-negative
	return PhysicsEq::pythagoras( c1-c2);
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

	//float value = std::max((float)0.0, (float)(pow(radius, 2) - pow(distance, 2)));
	//return pow(value, 3);

	float q = ( 1 / radius ) * distance;

	if (0 <= q && q <= 0.5) {

		return (6 * (pow(q, 3) - pow(q, 2))) + 1;
	}
	//else if (0.5 < q && q <= 1) {

	//	return 2 * pow((1 - q), 3);
	//}
	else {
		return 0;
	}
}

// This function is the derivative of the smoothing kernel Equation
//
// GRAD W(r-r',h)
// For particles further than the smoothing radius,
// This value is 0, and the pressure acting upon the
// particle is subsequently 0
float PhysicsEq::SmoothingKernelDerivative(float dst, float radius) {
	if (dst > radius) { return 0; }

	//float f = pow(radius, 2) - pow(dst, 2);
	//float scale = -24 / (M_PI * pow(radius, 8));
	//return scale * dst * pow(f, 2);

	float q = (1 / radius) * dst;

	if (0 <= q && q <= 0.5) {

		// Derivative of function within same smoothing function
		float scale = 240 / (7 * M_PI * pow(radius, 2));
		float kernelVal = (3 * pow(q, 2)) - (2 * q);

		return scale * kernelVal;
	}
	//else if (0.5 < q && q <= 1) {
	//	return 0;
	//	// Derivative of function within same smoothing function

	//	float scale = -240 / (7 * M_PI * pow(radius, 2));
	//	float kernelVal = pow(1 - q, 2);

	//	return scale * kernelVal;

	//}
	else {
		return 0;
	}




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

	float multiplier = (STIFFNESS_CONSTANT * REST_DENSITY) / EXPONENT;
	float pressureChange = pow((density / REST_DENSITY), EXPONENT) - 1;

	return multiplier * pressureChange;
}

// Inputs can be positive or negative, but squaring removes negative values

// Magnitude for 2D space
float PhysicsEq::pythagoras(glm::vec2 position) { return (float)sqrt(pow(position.x, 2) + pow(position.y, 2)); }

float PhysicsEq::pythagoras(glm::vec3 position) { return (float)sqrt(pow(position.x, 2) + pow(position.y, 2) + pow(position.z, 2)); }