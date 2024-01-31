#ifndef PARTICLE_H
#define PARTICLE_H

#include "PhysicsEq.h"
#include "glm/glm.hpp"
#include <iostream>

// UL, upper left, UR, upper right etc
struct SquareCoords {
	glm::vec2 m_TopLeft;
	glm::vec2 m_TopRight;
	glm::vec2 m_BottomLeft;
	glm::vec2 m_BottomRight;

	// Constructor for struct
	SquareCoords(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
		: m_TopLeft(a), m_TopRight(b), m_BottomLeft(c), m_BottomRight(d)
	{
	}

	void update(const glm::vec3 baseCoordinate,float radius) {
		m_TopLeft     = glm::vec2(baseCoordinate.x - radius, baseCoordinate.y + radius);
		m_TopRight	  = glm::vec2(baseCoordinate.x + radius, baseCoordinate.y + radius);
		m_BottomLeft  = glm::vec2(baseCoordinate.x - radius, baseCoordinate.y - radius);
		m_BottomRight = glm::vec2(baseCoordinate.x + radius, baseCoordinate.y - radius);
	}
};

class Particle
{
private:

	// VELOCITIES AND ACCELERATION HAVE EACH OWN RESPECTIVE AXIS
	glm::vec3 m_Acceleration;
	glm::vec3 m_Velocity;

	float m_Mass;
	float m_Radius;

public:

	enum Vector {
		Acceleration = 1,
		Velocity = 2,
	};

	glm::vec3 m_Position;
	SquareCoords m_Coords;

	float m_KernelRadius;

	// Default constructor
	Particle(
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
		float mass = 1,
		float radius = 1,
		float kernelRadius = 5,
		glm::vec3 vel = { 0.0f, 0.0f, 0.0f },
		glm::vec3 acc = { 0.0f, 0.0f, 0.0f }
	)
		: m_Position(pos),
		m_Velocity(vel),
		m_Acceleration(acc),
		m_Mass(mass),
		m_Radius(radius),
		m_KernelRadius(kernelRadius),
		m_Coords(
			glm::vec2(m_Position.x - radius, m_Position.y + radius), // TL
			glm::vec2(m_Position.x + radius, m_Position.y + radius), // TR
			glm::vec2(m_Position.x - radius, m_Position.y - radius), // BL
			glm::vec2(m_Position.x + radius, m_Position.y - radius)  // BR
		)
	{
	};
	
	// Copy constructor
	Particle(const Particle& particle)
		: m_Position(particle.m_Position),
		  m_Velocity(particle.m_Velocity),
		  m_Acceleration(particle.m_Acceleration),
		m_Mass(particle.m_Mass),
		m_Radius(particle.m_Radius),
		m_KernelRadius(particle.m_KernelRadius),
		m_Coords(
			glm::vec2(m_Position.x - particle.m_Radius, m_Position.y + particle.m_Radius), // TL
			glm::vec2(m_Position.x + particle.m_Radius, m_Position.y + particle.m_Radius), // TR
			glm::vec2(m_Position.x - particle.m_Radius, m_Position.y - particle.m_Radius), // BL
			glm::vec2(m_Position.x + particle.m_Radius, m_Position.y - particle.m_Radius)  // BR
		)
	{
		std::cout << "Copied!" << std::endl;
	}

	void update_Accel(float x = 0, float y = 0, float z = 0);
	void update_Vel();
	void update_Pos(float time);

	void bounce();
	
	float invert(float value);
	void invert(Vector type);
	void invert(glm::vec3 type);

	bool isMoving(float time);
	bool notMoving(float time);

	inline float calcKineticEnergy();

	inline float getRadius() { return m_Radius; }
	inline float getMass() { return m_Mass; }
	glm::vec3 getAcceleration() { return m_Acceleration; }
	glm::vec3 getVelocity() { return m_Velocity; };

	void setVelocity(float v);
};

#endif

