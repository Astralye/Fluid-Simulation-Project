#ifndef UNIFORMSPACEPARTITION_H
#define UNIFORMSPACEPARTITION_H

#include "glm/glm.hpp"
#include "Simulation/Rectangle.h"

class UniformSpacePartition {

private:
	glm::vec3 m_Position;

public:

	glm::ivec2 m_Cells;
	glm::vec2 m_CellSize;

	// We can pass in the rect container
	UniformSpacePartition() :
		m_Cells({ 0,0 }), m_CellSize({ 0,0 }), m_Position({ 0,0,0 })
	{};
	void SetContainer(RectangleContainer& cont);
	void SetContainer(RectangleContainer& cont, 
		glm::vec2 cellDimensions);

	inline glm::ivec2 getCells() { return m_Cells; }
	inline glm::vec2 getCellSize() { return m_CellSize; }
	inline glm::vec3 getPosition() { return m_Position; }
};

#endif