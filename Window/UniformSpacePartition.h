#ifndef UNIFORMSPACEPARTITION_H
#define UNIFORMSPACEPARTITION_H

#include <algorithm>

#include "glm/glm.hpp"
#include "Simulation/Rectangle.h"
#include "Simulation/Particle.h"
#include "Simulation/Particle.h"

#include "Settings.h"

class UniformSpacePartition {

private:
	glm::vec3 m_Position;

	struct spatialLookup {

		// Default constructor
		spatialLookup(int inputCell = 0, int inputIndex = 0)
			: cellKey(inputCell), particleIndex(inputIndex){}

		int particleIndex;
		int cellKey;
	};

public:

	glm::ivec2 m_Cells;
	glm::vec2 m_CellSize;
	std::vector<spatialLookup>* lookupList;

	Rectangle m_RenderSquare;

	// We can pass in the rect container
	UniformSpacePartition() :
		m_Cells({ 0,0 }), m_CellSize({ 0,0 }), m_Position({ 0,0,0 })
	{}

	void checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont);

	static bool compareCellID(const spatialLookup& a, const spatialLookup& b);
	void InitializeLookup();

	void SetContainer(RectangleContainer& cont);
	void SetContainer(RectangleContainer& cont, 
		glm::ivec2 cellDimensions);

	void defaultSize(RectangleContainer& cont, glm::vec2 cellSize);


	inline glm::vec3 getPosition() { return m_Position; }
};

#endif