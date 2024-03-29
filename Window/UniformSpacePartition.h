#ifndef UNIFORMSPACEPARTITION_H
#define UNIFORMSPACEPARTITION_H

#include "glm/glm.hpp"
#include "Simulation/Rectangle.h"
#include "Simulation/Particle.h"
#include "Simulation/Particle.h"

class UniformSpacePartition {

private:
	glm::vec3 m_Position;

public:

	glm::ivec2 m_Cells;
	glm::vec2 m_CellSize;

	Rectangle m_RenderSquare;

	// Pointer to array including every cell index.
	int *n_TotalCells;

	// We can pass in the rect container
	UniformSpacePartition() :
		m_Cells({ 0,0 }), m_CellSize({ 0,0 }), m_Position({ 0,0,0 })
	{};
	void SetContainer(RectangleContainer& cont);
	void SetContainer(RectangleContainer& cont, 
		glm::ivec2 cellDimensions);

	void defaultSize(RectangleContainer& cont, glm::vec2 cellSize);

	void checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont);
	void Dealloc();

	inline glm::vec3 getPosition() { return m_Position; }
};

#endif