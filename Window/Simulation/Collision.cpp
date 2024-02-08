#include "Collision.h"
#include "PhysicsEq.h"

#include "glm/glm.hpp"

#include <iostream>

void Collision::collisionResponse(Particle& A, Particle& B) {

	//glm::vec3 a_Accel = A->getAcceleration();
	//glm::vec3 a_Vel = A->getVelocity();

	//glm::vec3 b_Accel = B.getAcceleration();
	//glm::vec3 b_Vel   = B.getVelocity();

	// Find unit normal + tangent vectors
	//float a_XKe = PhysicsEq::kineticEnergy(A->getMass(), A->getVelocity().x);
	//float b_XKe = PhysicsEq::kineticEnergy(B->getMass(), B->getVelocity().x);

	//std::cout << "aX = " << a_XKe << "bX = " << b_XKe << std::endl;

	//float vel1 = (((A->getMass() - B->getMass()) /
	//	(A->getMass() + B->getMass())) * A->getVelocity().x)
	//	+ (((2 * B->getMass()) /
	//		(A->getMass() + B->getMass())) * B->getVelocity().x);

	//float vel2 = (((2 * B->getMass()) /
	//	(A->getMass() + B->getMass())) * A->getVelocity().x)
	//	+ (((A->getMass() - B->getMass()) /
	//		(A->getMass() + B->getMass())) * B->getVelocity().x);

	//A->setVelocity(0.0);

	//B->setVelocity(0.0);

	//A->invert(Particle::Acceleration);


	//std::cout << "Av = " << A->getVelocity().x << ", Bv = " << B->getVelocity().x <<  std::endl;
	//std::cout << "u1 = " << vel1 << ", u2 =" << vel2 << std::endl;
}

void Collision::collisionResponse(Particle& A, collisionType::Type type) {

	glm::vec3 flip{ false,false,false };

	switch (type) {
	case collisionType::Horizontal:

		if (ENABLE_GRAVITY) {

			if (A.getVelocity().x == 0) {
				A.setVelocity(0);
			}
			else {
				A.m_Position.y += 0.1;
			}

			A.bounce();
		}
		else {
			flip.y = true;
		}
		break;

	case collisionType::Vertical:
		flip.x = true;
		break;
	}

	A.invert(flip);
}

bool Collision::collisionDetection(Particle& A, Particle& B) {
	float distance = PhysicsEq::euclid_Distance(A.m_Position, B.m_Position);
	auto maxLength = A.getRadius() + B.getRadius();

	if (distance <= maxLength) {
		return true;
	}
	return false;
}

bool Collision::collisionDetection(Rectangle& A, Particle& B) {

	glm::vec4 clamp;

	clamp.x = PhysicsEq::clamp(B.m_Position.x, A.m_Position.x - (A.length / 2), A.m_Position.x + (A.length / 2));
	clamp.y = PhysicsEq::clamp(B.m_Position.y, A.m_Position.y - (A.height / 2), A.m_Position.y + (A.height/ 2));
	
	glm::vec4 difference = clamp - B.m_Position;

	float overlap = B.getRadius() - PhysicsEq::pythagoras(difference.x, difference.y);

	if (std::isnan(overlap)) {
		overlap = 0;
	}

	if (overlap > 0) {
		return true;
	}
	return false;
}

// Collision between a Rectangle and Particle
collisionType Collision::collisionDetection(RectangleContainer& A, Particle& B) {
	if (collisionDetection(A.m_SideA, B) || collisionDetection(A.m_SideC, B)) {
		return { true, collisionType::Horizontal };
	}

	if (collisionDetection(A.m_SideB, B) || collisionDetection(A.m_SideD, B)) {
		return { true, collisionType::Vertical };
	}
	
	return { false , collisionType::N_A };
}

