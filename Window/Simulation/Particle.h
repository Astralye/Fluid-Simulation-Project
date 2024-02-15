#ifndef PARTICLE_H
#define PARTICLE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "PhysicsEq.h"
#include "glm/glm.hpp"
#include <vector>

class Particle{
private:

	glm::vec3 m_Acceleration;
	glm::vec3 m_Velocity;

	float m_Mass;
	float m_Radius;
	float m_Density;

public:

	enum axis {
		x,
		y
	};

	enum Vector {
		Acceleration = 1,
		Velocity = 2,
	};

	glm::vec4 m_Position;

	glm::vec3 m_PredictedPos;
	glm::vec3 m_PredictedVelocity;
	glm::vec2 m_Vertices[4];

	// Default constructor
	Particle(
		glm::vec4 pos = { 0.0f, 0.0f, 0.0f, 0.0f },
		float mass = 1,
		float radius = 1,
		glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
		glm::vec3 acc = { 0.0f, 0.0f, 0.0f } )
	:
		m_Position(pos),
		m_Radius(radius),
		m_Acceleration(acc),
		m_Velocity(vel),
		m_Mass(mass),

		m_Density(0),
		m_PredictedPos({ 0,0,0 }),
		m_PredictedVelocity({ 0,0,0 })
	{
		m_Vertices[0] =
		{ m_Position.x - radius, m_Position.y + radius }; // TL

		m_Vertices[1] =
		{ m_Position.x + radius, m_Position.y + radius }; // TR

		m_Vertices[2] =
		{ m_Position.x + radius, m_Position.y - radius }; // BR

		m_Vertices[3] =
		{ m_Position.x - radius, m_Position.y - radius }; // BL
	}

	// Static
	// -------------------------------------------------


	// Static functions

	// Creates a cube of particles
	static void init_Cube(std::vector<Particle> *particleArray, float radius, float spacing);
	static void init_Random(std::vector<Particle> *particleArray, float radius);

	// SPH FUNCTIONS
	// This may need to change again for optimization,
	// but we can leave them here for now
	static void CalculateAllDensities(std::vector<Particle> *particleArray);
	static void CalculateDensity(std::vector<Particle> *arr, Particle &particle, int j);

	static void CalculateAllPressures(std::vector<Particle>* particleArray);
	static glm::vec2 CalculatePressureForce(std::vector<Particle> *arr, Particle & chosenParticle,int j);

	// Static variables

	static float KERNEL_RADIUS;
	static float particleProperties[MAX_PARTICLES];
	static float TARGET_DENSITY;

	// -------------------------------------------------


	// Update values
	void update_Accel();
	void update_Vel();
	void update_Pos();
	void update_PosPredicted();
	void update();

	// Getters
	void bounce();
	
	float invert(float value);
	void invert(Vector type);
	void invert(glm::vec3 type);

	// Getters
	inline float getRadius() { return m_Radius; }
	inline float getMass() { return m_Mass; }
	inline float getDensity() { return m_Density; }
	inline glm::vec3 getAcceleration() { return m_Acceleration; }
	inline glm::vec3 getVelocity() { return m_Velocity; }
	inline glm::vec3 getPredictedVelocity() { return m_PredictedVelocity; }

	

	// Setter
	void setVelocity(glm::vec2 vel);
	void setVelocity(float vel, axis type);

	void setPredictedVelocity(glm::vec3 pVel);

	void addVelocity(glm::vec2 vel);
	void setAcceleration(glm::vec2 acc);

	void setDensity(float den);

	bool operator==(const Particle& comp) const;
	bool operator!=(const Particle& comp) const;

};

#endif

