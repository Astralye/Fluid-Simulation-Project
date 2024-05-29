#ifndef SOURCESINK_H
#define SOURCESINK_H

#include <chrono>
#include <cstdlib>

#include "glm/glm.hpp"

#include "Simulation/Rectangle.h"
#include "Simulation/PhysicsEq.h"
#include "Simulation/Particle.h"
#include "Settings.h"


class Source {

private:

	bool hasReachedMax;

	std::chrono::time_point<std::chrono::system_clock> startTime, currentTime;
	std::chrono::duration<float> timeElapsed;

	glm::vec3 randomGenerateInRadius();
	float randomFloat();

public:
	
	glm::vec3 m_startingVelocity;
	float m_GenerationRadius;
	float m_GenerationRate;
		
	Source(
		glm::vec3 pos = { 0.0f,0.0f,0.0f },
		float genRate = 0, // in s
		float genRad = 1,
		glm::vec3 startVel = { 0.0f,0.0f, 0.0f }
	)
		:
		startTime(std::chrono::system_clock::now()),
		timeElapsed(0),

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
	bool canGenerate();

	void addParticle(uint16_t &nParticles, std::vector<Particle>* particleArray);


	// Updates the position of the source
	void update();
};

#endif