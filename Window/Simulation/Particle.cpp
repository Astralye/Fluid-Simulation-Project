#include <iostream>"

#include "Particle.h"
#include "Sim.h"

const bool ENABLE_GRAVITY = false;

// MAKE SURE EVERY MOVEMENT IS IN RESPECT TO A SINGLE SIMULATION STEP.

void Particle::update_Accel(float x, float y, float z) {

	// Do some calculations

	float a_X, a_Y, a_Z;

	float x_Jerk = 30;
	float y_Jerk = 30;

	a_X = m_Acceleration.x;
	a_Y = m_Acceleration.y;
	a_Z = m_Acceleration.z;

	// Jerk is the change of m_Acceleration, derivative of m_Acceleration.
	// Change of m_Acceleration is in proportion to simulation step.

	if (m_Velocity.x > 0 && m_Acceleration.x > 0) {
		x_Jerk = toNegative(x_Jerk);
	}

	if (m_Velocity.y > 0 && m_Acceleration.y > 0) {
		y_Jerk = toNegative(y_Jerk);
	}

	// Gravity

	if (ENABLE_GRAVITY) {
		if (abs(a_Y) < abs(PhysicsEq::GRAVITY) && m_Position.y > 0) {
			a_Y = a_Y + (-y_Jerk * Sim::SIMSTEP);
		}
		else {
			a_Y = PhysicsEq::GRAVITY;
		}
	}

	// Remove values close to zero
	if (abs(a_X) <= 0.5) { a_X = 0; }
	if (abs(a_Y) <= 0.1) { a_Y = 0; }

	m_Acceleration = glm::vec3(a_X, a_Y, a_Z);
}

// New m_Velocity = old m_Velocity + new m_Acceleration over t period
// V = u + at
void Particle::update_Vel() {

	// Initial Velocities
	float u_X, u_Y, u_Z, friction;

	u_X = m_Velocity.x;
	u_Y = m_Velocity.y;
	u_Z = m_Velocity.z;
	//friction = 0.9999;
	friction = 1;


	// if there is an acceleration, use it. If not , slow down by a fraction.

	if (m_Acceleration.x != 0) {
		u_X = u_X + (m_Acceleration.x * Sim::SIMSTEP);
	}
	else {
		u_X = u_X * friction;
	}

	if (m_Acceleration.y != 0) {
		u_Y = u_Y + (m_Acceleration.y * Sim::SIMSTEP);
	}
	else {
		u_Y = u_Y * friction;
	}

	//if (abs(u_Y) > abs(PhysicsEq::GRAVITY * m_Mass)) {
	//	if (u_Y > 0) {
	//		u_Y = PhysicsEq::GRAVITY * m_Mass;
	//	}
	//	u_Y = - PhysicsEq::GRAVITY * m_Mass;
	//}

	u_Z = u_Z + (m_Acceleration.y * Sim::SIMSTEP);
	
	// Remove values close to zero
	if (abs(u_X) < 0.1 && abs(m_Acceleration.x) == 0) { u_X = 0; }
	if (abs(u_Y) < 0.1 && abs(m_Acceleration.y) == 0) { u_Y = 0; }

	m_Velocity = glm::vec3(u_X, u_Y, u_Z);

}

// S = ut + 1/2(at)^2
// If m_Acceleration is negative, maintain distance, remove 1/2(at)^2
void Particle::update_Pos(float time) {
	// Retrieve velocities
	float x, y, z;

	x = m_Position.x;
	y = m_Position.y;
	z = m_Position.z;


	// CLEAN UP CODE THIS LOOKS VERY MESSY
	//
	//

	x = x + m_Velocity.x * Sim::SIMSTEP;

	//if (m_Acceleration.x <= 0) {
	//	// If negative m_Acceleration/ decceleration, distance moves back instead
	//	if (m_Velocity.x > 0){
	//		x += m_Velocity.x * Sim::SIMSTEP;
	//	}
	//}
	//else
	//{
	//	//x = x + ((m_Velocity.getX() * t) + (m_Acceleration.getX() / 2) * pow(t, 2)) * Sim::SIMSTEP;
	//}

	// S = vt - 1/2(at)^2
		
	// Movement due to m_Acceleration
	//float yAccel = (PhysicsEq::GRAVITY * pow(m_TimeSinceContact, 2)) / 2;
	y = y + m_Velocity.y * Sim::SIMSTEP ;

	// When bounce
	if (y <= 0) {
		y = 0;
		bounce();
	}

	if (m_Acceleration.z < 0) {
		// If negative m_Acceleration/ decceleration, distance moves back instead
		//z = z + (m_Velocity.getZ() * t) * Sim::SIMSTEP;
	}
	else
	{
		//z = z + ((m_Velocity.getZ() * t) + (m_Acceleration.getZ() / 2) * pow(t, 2)) * Sim::SIMSTEP;
	}

	m_Position = glm::vec3(x, y, z);
	m_Coords.update(m_Position,m_Radius);

	// update square coordinate pointss

	//std::cout << "X a:" << m_Acceleration.x << "  " << "V: " << m_Velocity.x;
	//std::cout << ", Y a:" << m_Acceleration.y << "  " << "V: " << m_Velocity.y << std::endl;
}

void Particle::bounce() {
	float bounceCoEff = 0.7;

	m_Acceleration.y = invert(m_Acceleration.y);
	m_Velocity.y = bounceCoEff * invert(m_Velocity.y);

	m_Velocity.x = m_Velocity.x * 0.7;
}

bool Particle::isMoving(float time) {
	/*
	if ((m_Velocity.getX() != 0 || m_Velocity.getY() != 0 || m_Velocity.getZ() != 0) || time >= 0) {
		//std::cout << "run" << std::endl;
		return true;
	}
	std::cout << "dont run" << std::endl;
	return false;
	*/
	return false;
}

bool Particle::notMoving(float time) {
	return !isMoving(time);
}

void Particle::setVelocity(float v)
{
	m_Velocity.x = v;
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
}

// Always returns Negative value
inline float Particle::toNegative(float value) {
	return -toPositive(value);
}

// Always returns positive value
inline float Particle::toPositive(float value) {
	return abs(value);
}
