#ifndef SOURCESINK_H
#define SOURCESINK_H

#include "Simulation/Rectangle.h"

#include "glm/glm.hpp"

#include "Simulation/Particle.h"
#include "Settings.h"

class Source {

private:

	float m_GenerationRate;
	float m_GenerationRadius;

	glm::vec3 m_startingVelocity;
	bool hasReachedMax;

public:
	Source(
		glm::vec3 pos = { 0.0f,0.0f,0.0f },
		float genRate = 0, // Per second
		float genRad = 1,
		glm::vec3 startVel = { 0.0f,0.0f, 0.0f }
	)
		:
		m_Position(pos),
		m_GenerationRate(genRate),
		m_GenerationRadius(genRad),
		hasReachedMax(false),
		m_startingVelocity(startVel),
		m_Quad(pos, genRad, genRad)
	{
	}

	glm::vec3 m_Position;
	Rectangle m_Quad;

	bool reachedMaxParticles(uint16_t current, int max);

	void addParticle(uint16_t &nParticles, std::vector<Particle>* particleArray);

	// Updates the position of the source
	void update();
};

#endif