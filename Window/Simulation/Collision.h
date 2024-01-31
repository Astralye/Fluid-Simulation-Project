#pragma once
#include "Particle.h"
#include "Simulation/Rectangle.h"

struct collisionType {
	bool m_isCollision;

	enum Type {
		N_A = 0,
		Vertical = 1,
		Horizontal = 2
	};

	Type type;

	collisionType(bool collision,Type value = Type::N_A)
		: m_isCollision(collision), type(value)
	{
	}

};

class Collision {

public:
	static void collisionResponse(Particle* A, Particle* B);
	static void collisionResponse(Particle* A, collisionType::Type type);



	static bool collisionDetection(Particle& A, Particle& B);
	static bool collisionDetection(Rectangle& A, Particle& B);
	static collisionType collisionDetection(RectangleContainer& A, Particle& B);
};