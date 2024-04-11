#include "SourceSink.h"

// Updates position of the source.
void Source::update()
{
	m_Quad.update_Size(2* m_GenerationRadius, 2* m_GenerationRadius);
	m_Quad.update_Position(m_Position);
	m_Quad.update_Vertices();
}

bool Source::reachedMaxParticles(uint16_t current, int max)
{
	return (current < max) ? false : true;
}

void Source::addParticle(uint16_t &nParticles, std::vector<Particle>* particleArray)
{
	if (reachedMaxParticles(nParticles, Settings::MAX_PARTICLES )) return;

	particleArray->emplace_back(
		glm::vec4(m_Position, 0.0f), 1.0f, 1.0f ,m_startingVelocity);
	
	nParticles++;
}
