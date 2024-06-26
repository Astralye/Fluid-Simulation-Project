#include "SPH.h"

void SPH::CalculateAllDensities(std::vector<Particle>* particleArray, std::vector<int> &particlesMaincell, std::vector<int>& particleIndices)
{	
	// All the particles within the center grid is compared to all the others
	int index;
	for (int i = 0; i < particlesMaincell.size(); i++) {
		index = particlesMaincell.at(i);

		glm::vec3 predictedVel = particleArray->at(index).getVelocity();
		
		if (Settings::ENABLE_GRAVITY) {
			predictedVel.y += (Settings::SIMSTEP * PhysicsEq::GRAVITY);
		}

		particleArray->at(index).setPredictedVelocity(predictedVel);
		particleArray->at(index).SetPredictedPosition();

		// Updates all density values
		SPH::CalculateDensity(particleArray, particleIndices, index);
	}
}

void SPH::CalculateAllDensities(std::vector<Particle>* particleArray)
{
	// Calculate predicted postion and velocity
	for (int i = 0; i < particleArray->size(); i++) {

		// Predictd velocity v*
		glm::vec3 predictedVel = particleArray->at(i).getVelocity();

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
	//float kernelNormalization = 40 / (7 * M_PI * pow(Particle::KERNEL_RADIUS, 2));

	// SUM
	for (int i = 0; i < arr->size(); i++) {
		if (i == j) { continue; }

		density += arr->at(i).getMass() * (PhysicsEq::SmoothingKernel(arr->at(i).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS));
	
	
	}
	arr->at(j).setDensity(density);
}
// Passing the entire particle array, particles within radius and the particle itself.
void SPH::CalculateDensity(std::vector<Particle>* arr, std::vector<int>& particleIndices, int j) {

	float density = 0;
	//float kernelNormalization = 40 / (7 * M_PI * pow(Particle::KERNEL_RADIUS, 2));
	int index;

	// SUM
	for (int i = 0; i < particleIndices.size(); i++) {
		index = particleIndices.at(i);
		if (index == j) { 
			continue;
		}

		density += arr->at(index).getMass() * PhysicsEq::SmoothingKernel(arr->at(index).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS);
	}
	arr->at(j).setDensity(density);
}

void SPH::CalculatePositionCollision(std::vector<Particle>* arr, RectangleContainer& container)
{
	for (int i = 0; i < arr->size(); i++) {


		Axis Axes = Collision::collisionDetection(container, arr->at(i));
		arr->at(i).bounce(Axes);
		arr->at(i).update();
	}
}

void SPH::CalculatePositionCollision(std::vector<Particle>* arr, std::vector<int>& particlesMaincell, RectangleContainer& container, bool isBorder)
{
	int index;
	for (int i = 0; i < particlesMaincell.size(); i++) {
		index = particlesMaincell.at(i);

		if (isBorder)
		{
			// Check for collision detection for each particle against the container.
			Axis Axes = Collision::collisionDetection(container, arr->at(index));
			arr->at(index).bounce(Axes);
		}

		// Only updates locations after resolving collisions.
		arr->at(index).update();
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
			// For non zero values
			glm::vec2 pressureAcceleration = pressureForce / particleArray->at(i).getDensity();
			particleArray->at(i).setAcceleration(pressureAcceleration + glm::vec2(0.0f, PhysicsEq::GRAVITY));

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

void SPH::CalculateAllPressures(std::vector<Particle>* particleArray, std::vector<int>& particlesMaincell, std::vector<int>& particleIndices)
{
	int index;
	// Calculates pressure and resultant velocity
	for (int i = 0; i < particlesMaincell.size(); i++) {
		index = particlesMaincell.at(i);
		glm::vec2 pressureForce = SPH::CalculatePressureForce(particleArray, particleIndices, index);
		glm::vec2 surfaceTensionForce = SPH::CalculateAdhesion(particleArray, particleIndices, index);

		glm::vec3 sum = particleArray->at(index).getPredictedVelocity();

		// F = MA --> A = F / M
		if (particleArray->at(index).getDensity() != 0) {
			glm::vec2 pressureAcceleration = pressureForce / particleArray->at(index).getDensity();
			glm::vec2 surfaceTensionAcceleration = -( surfaceTensionForce * PhysicsEq::COHESION_COEFF ) / particleArray->at(index).getDensity();

			// Just for displaying. Not using acceleration
			particleArray->at(index).setAcceleration(pressureAcceleration + surfaceTensionAcceleration);
			particleArray->at(index).setPressure(pressureForce);

			//pressure projection
			sum += Settings::SIMSTEP * glm::vec3(pressureAcceleration + surfaceTensionAcceleration, 0);
		}

		//// Gravity is Enabled
		if (Settings::ENABLE_GRAVITY) {
			sum.y += PhysicsEq::GRAVITY * Settings::SIMSTEP;
		}
		particleArray->at(index).setVelocity(sum);
	}
}

glm::vec2 SPH::CalculateAdhesion(std::vector<Particle>* particleArray, std::vector<int>& particlesMaincell, int j) {

	glm::vec2 sum = {0.0f,0.0f};

	int index;
	for (int i = 0; i < particlesMaincell.size(); i++) {
		index = particlesMaincell.at(i);
		if (index == j) { continue; }

		float mass = particleArray->at(index).getMass();

		glm::vec2 positionDifference = particleArray->at(j).m_PredictedPos - particleArray->at(index).m_PredictedPos;

		sum += mass * positionDifference * PhysicsEq::SmoothingKernel(particleArray->at(index).m_PredictedPos, particleArray->at(j).m_PredictedPos, Particle::KERNEL_RADIUS);
	}
	return sum;
}

glm::vec2 SPH::CalculateAdhesion(std::vector<Particle>* particleArray, int j) {

	glm::vec2 sum = { 0.0f,0.0f };

	for (int i = 0; i < particleArray->size(); i++) {

		float mass = particleArray->at(i).getMass();

		glm::vec2 positionDifference = particleArray->at(j).m_PredictedPos - particleArray->at(i).m_PredictedPos;

		sum += mass * positionDifference * PhysicsEq::SmoothingKernel(particleArray->at(i).m_PredictedPos, particleArray->at(j).m_PredictedPos, Particle::KERNEL_RADIUS);
	}
	return sum;
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

glm::vec2 SPH::CalculatePressureForce(std::vector<Particle>* arr, std::vector<int>& particleIndices, int j) {

	glm::vec2 pressureForce = { 0,0 };

	float localDensity = arr->at(j).getDensity();
	float localPressure = PhysicsEq::ConvertDensityToPressure(localDensity);

	// Pi / pi^2
	float localPressureDensity = localPressure / pow(localDensity, 2);

	int index;
	for (int i = 0; i < particleIndices.size(); i++) {
		index = particleIndices.at(i);	
		// Skips the code if the particle is itself or the density is 0
		if (index == j || arr->at(index).getDensity() == 0) { continue; }

		float dst = PhysicsEq::euclid_Distance(arr->at(index).m_PredictedPos, arr->at(j).m_PredictedPos);
		glm::vec2 dir = (arr->at(index).m_PredictedPos - arr->at(j).m_PredictedPos) / dst;

		float slope = PhysicsEq::SmoothingKernelDerivative(dst, Particle::KERNEL_RADIUS);
		float density = arr->at(index).getDensity();

		float externalPressure = PhysicsEq::ConvertDensityToPressure(density);
		float externalPressureDensity = externalPressure / pow(density, 2);

		pressureForce += arr->at(index).getMass() * (localPressureDensity + externalPressureDensity)
			* slope * dir;
	}
	
	return pressureForce;
}

void SPH::CalculateAllViscosities(std::vector<Particle>* arr, std::vector<int>& particlesMaincell, std::vector<int>& particleIndices)
{
	for (int i = 0; i < particlesMaincell.size(); i++) {
		CalculateViscosity(arr, particleIndices, particlesMaincell.at(i));
	}
}

void SPH::CalculateAllViscosities(std::vector<Particle>* arr)
{
	for (int i = 0; i < arr->size(); i++) {
		CalculateViscosity(arr, i);
	}
}

// Add a new velocity, the forces of other particles by a constant.
void SPH::CalculateViscosity(std::vector<Particle>* arr, std::vector<int>& particleIndices, int j) {

	glm::vec2 viscosityForce = { 0,0 };

	int index;
	for (int i = 0; i < particleIndices.size(); i++) {
		index = particleIndices.at(i);
		if (index == j) { continue; }

		glm::vec2 difference = {
			arr->at(index).getVelocity().x - arr->at(j).getVelocity().x,
			arr->at(index).getVelocity().y - arr->at(j).getVelocity().y,
		};

		// If viscosity forces are nan, do not calculate viscosity.
		// Causes particles of anomalous velocity to all disappear
		if (std::isnan(viscosityForce.x) || std::isnan(viscosityForce.y)) {
			return;
		}

		// When multiplying these values together, they cause a weird bug where the value is Nan.
		viscosityForce += difference * (PhysicsEq::SmoothingKernel(arr->at(index).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS));
	}
	arr->at(j).addVelocity(viscosityForce * PhysicsEq::VISCOSITY);
}


void SPH::CalculateViscosity(std::vector<Particle>* arr, int j) {

	glm::vec2 viscosityForce = { 0,0 };

	for (int i = 0; i < arr->size(); i++) {
		if (i == j) { continue; }

		glm::vec2 difference = {
			arr->at(i).getVelocity().x - arr->at(j).getVelocity().x,
			arr->at(i).getVelocity().y - arr->at(j).getVelocity().y, };

		viscosityForce += difference * (PhysicsEq::SmoothingKernel(arr->at(i).m_PredictedPos, arr->at(j).m_PredictedPos, Particle::KERNEL_RADIUS));
	}

	arr->at(j).addVelocity(viscosityForce * PhysicsEq::VISCOSITY);
}

