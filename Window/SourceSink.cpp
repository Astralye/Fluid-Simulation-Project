#include "SourceSink.h"

// Updates position of the source.
void Source::update()
{
	m_Quad.update_Size(2* m_GenerationRadius, 2* m_GenerationRadius);
	m_Quad.update_Position(m_Position);
	m_Quad.update_Vertices();
}

glm::vec3 Source::randomGenerateInRadius()
{
	// Values are between -Radius to +Radius.
	glm::vec3 randomOffet = {
		randomFloat() - m_GenerationRadius,
		randomFloat() - m_GenerationRadius,
		0 // Value is set to 0 because in 2D space. The third vector is calculated in the euclidean distance and should be zero for 2D simulation.
	};

	return m_Position + randomOffet;
}

// Random float between 0 and 2R
float Source::randomFloat()
{
	return static_cast<float> (rand()) / (static_cast<float> (RAND_MAX) / (2 * m_GenerationRadius));
}

bool Source::reachedMaxParticles(uint16_t current, int max)
{
	return (current < max) ? false : true;
}

bool Source::canGenerate()
{
	float timeAtPreviousFrame, timeCurrent, timeSincePreviousFrame;
	std::chrono::duration<float> timeElapsedLocal;

	// The current time was calculated the previous frame.
	timeAtPreviousFrame = timeElapsed.count();
	currentTime = std::chrono::system_clock::now();

	timeElapsedLocal = currentTime - startTime;
	timeCurrent = timeElapsedLocal.count();

	timeSincePreviousFrame = timeCurrent - timeAtPreviousFrame;

	if (timeSincePreviousFrame >= m_GenerationRate) {
		timeElapsed = timeElapsedLocal;
		return true;
	}

	return false;
}

void Source::addParticle(uint16_t &nParticles, std::vector<Particle>* particleArray)
{
	if (reachedMaxParticles(nParticles, Settings::MAX_PARTICLES )) return;

	if (!canGenerate()) { return; }

	particleArray->emplace_back(
		glm::vec4(randomGenerateInRadius(), 0.0f), 1.0f, Particle::PARTICLE_RADIUS, m_startingVelocity);
	
	nParticles++;
}
