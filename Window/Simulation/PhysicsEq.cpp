// Standard libraries
#include <cmath>
#include <algorithm>
#include <iostream>

// Self made
#include "PhysicsEq.h"


// Static variables
float PhysicsEq::REST_DENSITY = 0.5f;
float PhysicsEq::STIFFNESS_CONSTANT = 50000.0f;
float PhysicsEq::EXPONENT = 2.6f;

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


	return STIFFNESS_CONSTANT * (std::max(density - REST_DENSITY, 0.0f));
}

// Inputs can be positive or negative, but squaring removes negative values

// Magnitude for 2D space
float PhysicsEq::pythagoras(float a, float b) { return (float)sqrt(pow(a, 2) + pow(b, 2)); }
// Magnitude for 3D space
float PhysicsEq::pythagoras(float a, float b, float c) { return (float) sqrt(pow(a, 2) + pow(b, 2) + pow(c,2)); }

