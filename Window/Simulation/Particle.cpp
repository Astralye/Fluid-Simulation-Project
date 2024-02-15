#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Settings.h"
#include "Particle.h"

// Static variables
float Particle::KERNEL_RADIUS = 0.0f;
float Particle::particleProperties[MAX_PARTICLES] = { 0 };

// Static functions
// ---------------------------------------------------------------------------------------------------


// Particle Initializers

void Particle::init_Cube(std::vector<Particle> *particleArray, float radius, float spacing)
{

	Particle::KERNEL_RADIUS = 4 * radius;

	// 8 bit -> 256^2 = max 65k particles
	uint16_t column, row;
	float squareDimension;

	glm::vec2 offset, position, max_Size;
	glm::vec2 containerCenter = { -50.0f,50.0f };

	// MAKE SURE EVERY MOVEMENT IS IN RESPECT TO A SINGLE SIMULATION STEP.

	squareDimension = (float)sqrt(MAX_PARTICLES);

	offset = { containerCenter.x / 2, containerCenter.y / 2 };
	max_Size = { (int)ceil(squareDimension), (int)floor(squareDimension) };

	// Set base values
	column = 0;
	row = 0;

	for (int i = 0; i < MAX_PARTICLES; i++) {
		position = { row * (2 * radius + spacing),
					column * (2 * radius + spacing)};
		row++;

		if (row == max_Size.x) {
			column++;
			row = 0;
		}
		particleArray->emplace_back(
			glm::vec4(position.x + offset.x, position.y + offset.y, 0.0f, 0.0f), 1.0f, radius);
	}
}
void Particle::init_Random(std::vector<Particle> *particleArray, float radius) {


	Particle::KERNEL_RADIUS = 8 * radius;

	int maxVelocity = 20;

	for (int i = 0; i < MAX_PARTICLES; i++) {

		particleArray->emplace_back(
			glm::vec4(8.0f + rand() % 85, 12.0f + rand() % 70, 0.0f,0.0f), 1.0f, radius
			//glm::vec3((rand() % maxVelocity), (rand() % maxVelocity), 0.0f),
		);
	}
}

// If the positions are the same. This doesn't exactly work if two
// particles are in the exact same spot, but the likelihood of that
// happening are close to zero
bool Particle::operator==(const Particle& comp) const {
	return (this->m_Position == comp.m_Position);
}

// ---------------------------------------------------------------------------------------------------


void Particle::CalculateAllDensities(std::vector<Particle>* particleArray)
{
	// Calculate predicted postion and velocity
	for (int i = 0; i < particleArray->size(); i++) {

		// Predictd velocity v*
		glm::vec3 predictedVel = particleArray->at(i).getVelocity() + (SIMSTEP * particleArray->at(i).getAcceleration());

		if(ENABLE_GRAVITY){ 
			predictedVel.y += (SIMSTEP * PhysicsEq::GRAVITY);
		}

		particleArray->at(i).setPredictedVelocity(predictedVel);
		particleArray->at(i).update_PosPredicted();

		// Updates all density values
		Particle::CalculateDensity(particleArray, particleArray->at(i), i);
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
void Particle::CalculateDensity(std::vector<Particle>* arr, Particle &chosenParticle,int j) {

	float density = 0;

	// This current function is O(n^2), need to improve upon it.

	float kernelNormalization = 40 / (7 * M_PI * pow(Particle::KERNEL_RADIUS, 2));

	// SUM
	for (int i = 0; i < arr->size(); i++) {
		if (arr->at(i) == chosenParticle) { continue; }

		density += arr->at(i).getMass() * ( PhysicsEq::SmoothingKernel(arr->at(i).m_PredictedPos, chosenParticle.m_PredictedPos, Particle::KERNEL_RADIUS) / kernelNormalization);
	}

	chosenParticle.setDensity(density);
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

void Particle::CalculateAllPressures(std::vector<Particle>* particleArray)
{		
	// Calculates pressure and resultant velocity
	for (int i = 0; i < particleArray->size(); i++) {
		glm::vec2 pressureForce = Particle::CalculatePressureForce(particleArray, particleArray->at(i), i);
		glm::vec3 sum = particleArray->at(i).getPredictedVelocity();

		// F = MA --> A = F / M
		if (particleArray->at(i).getDensity() != 0) {
			glm::vec2 pressureAcceleration = pressureForce / particleArray->at(i).getDensity();

			//pressure projection
			sum += (SIMSTEP * glm::vec3(pressureAcceleration, 0));
		}

		// Gravity is Enabled
		if (ENABLE_GRAVITY) {
			sum.y += SIMSTEP * PhysicsEq::GRAVITY;
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
glm::vec2 Particle::CalculatePressureForce(std::vector<Particle>* arr, Particle& chosenParticle, int j) {

	glm::vec2 pressureForce = { 0,0 };

	float localDensity = chosenParticle.getDensity();
	float localPressure = PhysicsEq::ConvertDensityToPressure(localDensity);
	
	// Pi / pi^2
	float localPressureDensity = localPressure / pow(localDensity, 2);

	for (int i = 0; i < arr->size(); i++) {
		// Skips the code if the particle is itself or the density is 0
		if (arr->at(i) == chosenParticle || arr->at(i).getDensity() == 0) { continue; }

		float dst = PhysicsEq::euclid_Distance(arr->at(i).m_PredictedPos, chosenParticle.m_PredictedPos);
		glm::vec2 dir = (arr->at(i).m_PredictedPos - chosenParticle.m_PredictedPos) / dst;

		float density = arr->at(i).getDensity();
		float externalPressure = PhysicsEq::ConvertDensityToPressure(density);

		float externalPressureDensity = externalPressure / pow(density, 2);

		pressureForce += arr->at(i).getMass() * ( localPressureDensity + externalPressureDensity)
			* PhysicsEq::SmoothingKernelDerivative(dst, Particle::KERNEL_RADIUS) * dir;
	}

	return pressureForce;
}



/*
*	Updates the acceleration of a particle
* 
*	Jerk is the derivative of acceleration.
*	Change of acceleration is in proportion to simulation step.
*/
void Particle::update_Accel() {

	glm::vec3 accel{ m_Acceleration.x, m_Acceleration.y, m_Acceleration.z };
	glm::vec3 jerk{ 30.0f, 30.0f, 0.0f };

	// Acceleration from particle collisions can cause spikes in numbers;
	// This dampens the values
	float CHAOS_MULTIPLIER = 0.1f;

	if (m_Velocity.x > 0 && m_Acceleration.x > 0) {
		jerk.x = PhysicsEq::toNegative(jerk.x);
	}

	if (m_Velocity.y > 0 && m_Acceleration.y > 0) {
		jerk.y = PhysicsEq::toNegative(jerk.y);
	}

	// Remove values close to zero
	if (abs(accel.x) <= 0.5f) { accel.x = 0; }
	if (abs(accel.y) <= 0.1f) { accel.y = 0; }

	m_Acceleration = accel;
}

/*
*	Updates velocity of a particle
* 
*	=====================================================
* 
*	if there is an acceleration, use the SUVAT equation
*
*	   V	=	u	  +		a * t
*	-------------------------------------
*	New Vel  Old Vel	Acceleration Time
*	
* 
*	if not, slow down particle via friction
*	   V	=   u	 *	 friction
* 
*	======================================================
* 
*/
void Particle::update_Vel() {

	glm::vec3 vel{ m_Velocity.x, m_Velocity.y, m_Velocity.z };
	
	float friction = 1;
	//float friction = 0.8;

	if (m_Acceleration.x != 0) { vel.x += (m_Acceleration.x * SIMSTEP); }
	else { vel.x *= friction; }

	if (m_Acceleration.y != 0) { vel.y += (m_Acceleration.y * SIMSTEP); }
	else { vel.y *= friction; }

	if (m_Acceleration.z != 0) { vel.z += (m_Acceleration.z * SIMSTEP); }
	else { vel.z *= friction; }

	// Remove values close to zero
	if (abs(vel.x) < 0.1f && abs(m_Acceleration.x) == 0) { vel.x = 0; }
	if (abs(vel.y) < 0.1f && abs(m_Acceleration.y) == 0) { vel.y = 0; }
	if (abs(vel.z) < 0.1f && abs(m_Acceleration.z) == 0) { vel.z = 0; }

	m_Velocity = vel;
}

/*
*	Updates position of a particle
*
*	=====================================================
* 
*	SUVAT equation relating time and movement:
* 
*	   S	=	 u	  *	 t    +  1/2 ( a  *	  t )^2
*	----------------------------------------------
*	  Pos	 Velocity	Time	Acceleration Time
* 
* 
* 
*	There is a problem with using acceleration and time,
*	such that it assumes a body is moving from point A to B
*	However, we cannot predict where point B will be after collisions,
*	and as such, it has to be omitted, leaving us instead with
*	
*	   S	=	 u	  *	 t 
*	-------------------------
*	  Pos	 Velocity	Time	
* 
*	======================================================
*
*/
void Particle::update_PosPredicted() {
	glm::vec3 pos{ m_Position.x, m_Position.y, m_Position.z };

	//pos.x += m_Velocity.x * SIMSTEP;
	//pos.y += m_Velocity.y * SIMSTEP;
	//pos.z += m_Velocity.z * SIMSTEP;

	pos.x += m_PredictedVelocity.x * SIMSTEP;
	pos.y += m_PredictedVelocity.y * SIMSTEP;
	pos.z += m_PredictedVelocity.z * SIMSTEP;


	m_PredictedPos = glm::vec4(pos, 0.0f);
}

void Particle::update_Pos() {
	glm::vec3 pos{ m_Position.x, m_Position.y, m_Position.z };

	pos.x += m_Velocity.x * SIMSTEP;
	pos.y += m_Velocity.y * SIMSTEP;
	pos.z += m_Velocity.z * SIMSTEP;

	m_Position = glm::vec4(pos, 0.0f);
}

// Updates all the accelerations, velocities and positions before changing the vertex positions.
void Particle::update(){
	//update_Accel();
	//update_Vel();
	update_Pos();

	m_Vertices[0] =
		{ m_Position.x - m_Radius, m_Position.y + m_Radius }; // TL

	m_Vertices[1] =
		{ m_Position.x + m_Radius, m_Position.y + m_Radius }; // TR

	m_Vertices[2] =
		{ m_Position.x + m_Radius, m_Position.y - m_Radius }; // BR

	m_Vertices[3] =
		{ m_Position.x - m_Radius, m_Position.y - m_Radius }; // BL

}


void Particle::bounce() {
	float bounceCoEff = 0.7f;

	m_Acceleration.y = invert(m_Acceleration.y) * ((float)rand()) / RAND_MAX;
	m_Velocity.y = bounceCoEff * invert(m_Velocity.y) * (((float)rand()) / RAND_MAX) ;

	m_Velocity.x = m_Velocity.x * 0.7f;
}

// Inverts value
float Particle::invert(float value)
{
	if (value < 0) { return abs(value); }
	return -value;
}

void Particle::invert(Vector type)
{
	if (type == Vector::Acceleration) {
		m_Acceleration.x = -m_Acceleration.x;
	}else
	{
		m_Velocity.x = -m_Velocity.x;
	}
}

void Particle::invert(glm::vec3 type)
{
	if (type.x) {
		m_Velocity.x = -m_Velocity.x;
		m_Acceleration.x = -m_Acceleration.x;
	}

	if (type.y) {
		m_Velocity.y = -m_Velocity.y;
		m_Acceleration.y = -m_Acceleration.y;
	}

	if (type.z) {
		m_Velocity.z = -m_Velocity.z;
		m_Acceleration.z = -m_Acceleration.z;
	}
}

void Particle::setVelocity(glm::vec2 vel) {
	m_Velocity.x = vel.x;
	m_Velocity.y = vel.y;
}

void Particle::setVelocity(float vel,axis type) {

	if (type == axis::x) {
		m_Velocity.x = vel;
	}
	else {
		m_Velocity.y = vel;
	}
}

void Particle::setPredictedVelocity(glm::vec3 pVel)
{
	m_PredictedVelocity = pVel;
}

void Particle::addVelocity(glm::vec2 vel) {
	m_Velocity.x += vel.x;
	m_Velocity.y += vel.y;
}

void Particle::setAcceleration(glm::vec2 acc) {
	m_Acceleration = glm::vec3(acc, 0);
}

void Particle::setDensity(float den)
{
	m_Density = den;
}
