#ifndef PARTICLE_H
#define PARTICLE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "PhysicsEq.h"
#include "glm/glm.hpp"
#include "Rectangle.h"
#include "Settings.h"

#include <vector>

// Particle Class
// -------------------------------------------------------------------------------------

class Particle{
private:

	glm::vec3 m_Acceleration;
	glm::vec3 m_Velocity;

	glm::vec2 m_Pressure;

	float m_Mass;
	float m_Radius;
	float m_Density;

public:

	enum axis {
		x,
		y
	};

	enum Vector {
		V_Acceleration,
		V_Velocity,
	};

	enum DebugType {
		D_Velocity,
		D_Density,
		D_Pressure
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
	static void CalculatePositionCollision(std::vector<Particle>* arr, RectangleContainer& container);

	static void CalculateAllPressures(std::vector<Particle>* particleArray);
	static glm::vec2 CalculatePressureForce(std::vector<Particle> *arr, Particle &chosenParticle,int j);

	static void CalculateAllViscosities(std::vector<Particle>* particleArray);
	static glm::vec2 CalculateViscosity(std::vector<Particle>* particleArray, Particle &chosenParticle);

	// Static variables

	static float KERNEL_RADIUS;
	static float particleProperties[Settings::MAX_PARTICLES];

	static DebugType Debug;

	// -------------------------------------------------

	// Update values
	void update_Accel();
	void update_Vel();
	void update_Pos();
	void update_PosPredicted();
	void update();

	void bounce();
	
	float invert(float value);
	void invert(Vector type);
	void invert(glm::vec3 type);

	glm::vec4 DebugColour();

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
	void setPressure(glm::vec2 pressure);

	bool operator==(const Particle& comp) const;
	bool operator!=(const Particle& comp) const;

};


// Collision Class
// I have made the decision to put collsion here 
// Makes the most sense, as particles can only be having collision detection
// Removes the need for cyclical includes and bugs
// -------------------------------------------------------------------------------

struct collisionType {

	enum Type { N_A, Vertical, Horizontal };

	bool m_isCollision;
	Type type;

	collisionType(bool collision, Type value = Type::N_A)
		: m_isCollision(collision), type(value) {}

};

class Collision {

public:
	static void collisionResponse(Particle& A, collisionType::Type type);

	static bool collisionDetection(Particle& A, Particle& B);
	static bool collisionDetection(Rectangle& A, Particle& B);
	static collisionType collisionDetection(RectangleContainer& A, Particle& B);
};

#endif

