#ifndef UNIFORMSPACEPARTITION_H
#define UNIFORMSPACEPARTITION_H

#include <algorithm>

#include "glm/glm.hpp"
#include "Simulation/Rectangle.h"
#include "Simulation/Particle.h"
#include "Simulation/Particle.h"

#include "Statistics.h"

#include "SPH.h"

#include "Settings.h"

// Paralell processing
#include <omp.h>

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
	std::vector<int>* startIndex;

	Rectangle m_RenderSquare;

	// We can pass in the rect container
	UniformSpacePartition() :
		m_Cells({ 0,0 }), m_CellSize({ 0,0 }), m_Position({ 0,0,0 })
	{}

	inline int coordToIndex(glm::vec2 coord);
	inline glm::vec2 indexToCoord(int index);

	void checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont);
	void getNeighbourParticles(std::vector<Particle>* particleArray, RectangleContainer& cont);
	void neighbourCells(std::vector<Particle>* particleArray, int index, RectangleContainer& cont);

	bool isBorderCell(glm::vec2 index);

	static bool compareCellID(const spatialLookup& a, const spatialLookup& b);
	void InitializeLookup();

	void SetContainer(RectangleContainer& cont);
	void SetContainer(RectangleContainer& cont, 
		glm::ivec2 cellDimensions);

	void defaultSize(RectangleContainer& cont, glm::vec2 cellSize);


	inline glm::vec3 getPosition() { return m_Position; }
};

#endif