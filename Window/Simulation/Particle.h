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

	enum Vector {
		Acceleration = 1,
		Velocity = 2,
	};

	glm::vec4 m_Position;
	glm::vec2 m_Vertices[4];

	float m_KernelRadius;

	// Default constructor
	Particle(
		glm::vec4 pos = { 0.0f, 0.0f, 0.0f, 0.0f },
		float mass = 1,
		float radius = 1,
		glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
		glm::vec3 acc = { 0.0f, 0.0f, 0.0f }
	)
		:
		m_Position(pos),
		m_Radius(radius),
		m_Acceleration(acc),
		m_Mass(mass),
		m_Density(0)
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

	// Static functions
	// -------------------------------------------------
	// Creates a cube of particles
	static void init_Cube(std::vector<Particle> &particleArray, float radius, float spacing);

	// Creates a particles with random positions and velocities
	static void init_Random(std::vector<Particle>& particleArray, float radius);

	static float CalculateDensity(std::vector<Particle> &arr, Particle &particle);
	static float CalculateProperty(std::vector<Particle>& arr, Particle& particle);
	static glm::vec2 CalculatePropertyGradient(std::vector<Particle>& arr, Particle& chosenParticle);

	static float ExampleFunction(glm::vec2 pos);


	// Static variables
	static float KERNEL_RADIUS;
	static float particleProperties[MAX_PARTICLES];

	// -------------------------------------------------


	// Update values
	void update_Accel();
	void update_Vel();
	void update_Pos();
	void update();

	// Getters
	void bounce();
	
	float invert(float value);
	void invert(Vector type);
	void invert(glm::vec3 type);

	bool isMoving(float time);
	bool notMoving(float time);

	// Getters
	inline float getRadius() { return m_Radius; }
	inline float getMass() { return m_Mass; }
	inline float getDensity() { return m_Density; }
	inline glm::vec3 getAcceleration() { return m_Acceleration; }
	inline glm::vec3 getVelocity() { return m_Velocity; }

	

	// Setter
	void setVelocity(float vel);
	void setDensity(float den);

	bool operator==(const Particle& comp) const;
	bool operator!=(const Particle& comp) const;

};

#endif

