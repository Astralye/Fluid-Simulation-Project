#include "SPH.h"


void SPH::CalculateAllDensities(std::vector<Particle>* particleArray)
{

	// Calculate predicted postion and velocity
	for (int i = 0; i < particleArray->size(); i++) {

		// Predictd velocity v*
		glm::vec3 predictedVel = particleArray->at(i).getVelocity() + (Settings::SIMSTEP * particleArray->at(i).getAcceleration());

		if (Settings::ENABLE_GRAVITY) {
			predictedVel.y += (Settings::SIMSTEP * PhysicsEq::GRAVITY);
		}

		particleArray->at(i).setPredictedVelocity(predictedVel);
		particleArray->at(i).SetPredictedPosition();

		// Updates all density values
		SPH::CalculateDensity(particleArray, i);
	}
}

/*
*	Calculate the density for a specified particle
*
*	======================================================
*
*	   p	= SIGMA [ mass * W(r - r',h) ]
*	--------------------------------------
*	Density	   SUM			   Kernel
*
*	To maintain densities at any kernel radius, the value
*	before added is divided by the volume, turning it into:
*
*	p ( Density ) = SIGMA ( SUM ) of [ [mass * W(r - r',h)] / volume ]
*
*	=======================================================
*/
void SPH::CalculateDensity(std::vector<Particle>* arr, int j) {

	float density = 0;
	float kernelNormalization = 40 / (7 * M_PI * pow(Particle::KERNEL_RADIUS, 2));

	// SUM
	for (int i = 0; i < arr->size(); i++) {
		if (i == j) { continue; }

		density += arr->at(i).getMass() * (PhysicsEq::SmoothingKernel(arr->at(i).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS) / kernelNormalization);
	}

	arr->at(j).setDensity(density);
}

void SPH::CalculatePositionCollision(std::vector<Particle>* arr, RectangleContainer& container)
{
	for (int i = 0; i < arr->size(); i++) {

		arr->at(i).update();

		// Check for collision detection for each particle against the container.
		auto collide = Collision::collisionDetection(container, arr->at(i));

		if (collide.m_isCollision) {
			Collision::collisionResponse(arr->at(i), collide.type);
		}
	}
}

/*
*	Calculate the density for a specified particle
*
*	========================================================
*
*	  A(x)	 = SIGMA,i [ A * mass /	p	* W(r - r',h) ]
*	---------------------------------------------------
*	Property    SUM_i	 A_i	Density_i	Kernel
*
*	Where A is the particle Property
*
*	When substituting the A to find density, with p,
*	this cancels out and giving us the equation from
*
*	- CalculateDensity()
*
*	=========================================================
*
*/
void SPH::CalculateAllPressures(std::vector<Particle>* particleArray)
{
	// Calculates pressure and resultant velocity
	for (int i = 0; i < particleArray->size(); i++) {
		glm::vec2 pressureForce = SPH::CalculatePressureForce(particleArray, i);
		glm::vec3 sum = particleArray->at(i).getPredictedVelocity();

		// F = MA --> A = F / M
		if (particleArray->at(i).getDensity() != 0) {
			glm::vec2 pressureAcceleration = pressureForce / particleArray->at(i).getDensity();

			//pressure projection
			sum += (Settings::SIMSTEP * glm::vec3(pressureAcceleration, 0));
		}

		// Gravity is Enabled
		if (Settings::ENABLE_GRAVITY) {
			sum.y += Settings::SIMSTEP * PhysicsEq::GRAVITY;
		}

		particleArray->at(i).setVelocity(sum);
	}
}

/*
*	Property Gradient.
*	Tells us where the highest gradient point is pointed towards
*
*	==============================================
*
*	A(x) = SIGMA,i [ Ai * m/p * GRAD W()
*
*/
glm::vec2 SPH::CalculatePressureForce(std::vector<Particle>* arr, int j) {

	glm::vec2 pressureForce = { 0,0 };

	float localDensity = arr->at(j).getDensity();
	float localPressure = PhysicsEq::ConvertDensityToPressure(localDensity);

	// Gets rid of zero values
	if (localDensity == 0) {
		localDensity = 0.01f;
	}

	// Pi / pi^2
	float localPressureDensity = localPressure / pow(localDensity, 2);

	for (int i = 0; i < arr->size(); i++) {
		// Skips the code if the particle is itself or the density is 0
		if (i == j || arr->at(i).getDensity() == 0) { continue; }

		float dst = PhysicsEq::euclid_Distance(arr->at(i).m_PredictedPos, arr->at(j).m_PredictedPos);
		glm::vec2 dir = (arr->at(i).m_PredictedPos - arr->at(j).m_PredictedPos) / dst;

		float density = arr->at(i).getDensity();
		float externalPressure = PhysicsEq::ConvertDensityToPressure(density);

		float externalPressureDensity = externalPressure / pow(density, 2);

		pressureForce += arr->at(i).getMass() * (localPressureDensity + externalPressureDensity)
			* PhysicsEq::SmoothingKernelDerivative(dst, Particle::KERNEL_RADIUS) * dir;
	}

	arr->at(j).setPressure(pressureForce);
	return pressureForce;
}

void SPH::CalculateAllViscosities(std::vector<Particle>* arr)
{
	for (int i = 0; i < arr->size(); i++) {
		arr->at(i).addVelocity(CalculateViscosity(arr, i));
	}
}

glm::vec2 SPH::CalculateViscosity(std::vector<Particle>* arr, int j) {

	glm::vec2 viscosityForce = { 0,0 };

	for (int i = 0; i < arr->size(); i++) {
		if (i == j) { continue; }

		glm::vec2 difference = {
			arr->at(i).getVelocity().x - arr->at(j).getVelocity().x,
			arr->at(i).getVelocity().y - arr->at(j).getVelocity().y, };

		viscosityForce += difference * (PhysicsEq::SmoothingKernel(arr->at(i).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS));
	}

	return viscosityForce * PhysicsEq::VISCOSITY;
}

