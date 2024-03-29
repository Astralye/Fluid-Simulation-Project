#include "UniformSpacePartition.h"

// Updating the arrays would be complicated for implementation due to movement of particles and needing to reset values
void UniformSpacePartition::checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont)
{
	/*
		This is run per frame.
		We would assume that the grid arrangment doesn't change per frame.
		Fixed size only when simulation is running.
		If the simulation is paused/ reset, the cell size can change.
	*/

	// Sets all the values back to 0.
	lookupList->clear();

	// If we normalized positions to be only inside the box, we can easily calculate which index it lies at
	// Current positions are in worldspace,
	glm::vec2 xbounds = { cont.m_Position.x - (cont.m_Length / 2) ,cont.m_Position.x + (cont.m_Length / 2) };
	glm::vec2 ybounds = { cont.m_Position.y - (cont.m_Height / 2) ,cont.m_Position.y + (cont.m_Height / 2) };

	for (int i = 0; i < particleArray->size(); i++) {
		glm::vec4 particlePos = particleArray->at(i).m_Position;
		int cellIndex;

		// Bounds checking
		if ((xbounds.x <= particlePos.x && particlePos.x <= xbounds.y) &&
			(ybounds.x <= particlePos.y && particlePos.y <= ybounds.y)) {

			// position - smallbound
			glm::vec2 localPos = { 
				particlePos.x - xbounds.x,
				ybounds.y - particlePos.y
			};

			localPos = {
				(float)floor(localPos.x / m_CellSize.x),
				(float)floor(localPos.y / m_CellSize.y)
			};

			cellIndex = localPos.y * m_Cells.x;
			cellIndex += localPos.x;

			// If the vector size is not full, emplace back
			lookupList->emplace_back(cellIndex,i);
		}
	}

	// Sorts by cellID
	std::sort(lookupList->begin(), lookupList->end(), compareCellID);

	// Sorts vector based on cell key.
	for (int i = 0; i < lookupList->size(); i++) {
		std::cout << "i: " << i << " cellkey: " << lookupList->at(i).cellKey << " particleID" << lookupList->at(i).particleIndex << std::endl;
	}

	// Still needs a start indices for checking where to look in the lookup list.

	// For checking nearby cells, needs to use the coordinates, rather than the index.
	// Makes calculating the cell index more easier.
}

bool UniformSpacePartition::compareCellID(const spatialLookup&a, const spatialLookup&b)
{
	return a.cellKey < b.cellKey;
}


void UniformSpacePartition::InitializeLookup()
{
	lookupList = new std::vector<spatialLookup>;
}

void UniformSpacePartition::defaultSize(RectangleContainer& cont, glm::vec2 cellSize) {
	m_CellSize = cellSize;
	m_Cells = { cont.m_Length / m_CellSize.x,cont.m_Height / m_CellSize.y };

	glm::vec3 start = {
		cont.m_SideD.m_Position.x + (m_CellSize.x / 2),
		cont.m_SideC.m_Position.y + (m_CellSize.y / 2),
		0 };

	m_RenderSquare.update_Size(Particle::KERNEL_RADIUS, Particle::KERNEL_RADIUS);
	m_RenderSquare.update_Position(start);
	m_RenderSquare.update_Vertices();
}

void UniformSpacePartition::SetContainer(RectangleContainer& cont) {
	m_CellSize = { cont.m_Length / m_Cells.x, cont.m_Height / m_Cells.y };
	m_Position = cont.m_Position;

	glm::vec3 start = {
		cont.m_SideD.m_Position.x + (m_CellSize.x / 2),
		cont.m_SideC.m_Position.y + (m_CellSize.y / 2),
		0 };

	m_RenderSquare.update_Size(m_CellSize.x, m_CellSize.y);
	m_RenderSquare.update_Position(start);
	m_RenderSquare.update_Vertices();
}

void UniformSpacePartition::SetContainer(RectangleContainer& cont, glm::ivec2 cellDimensions) {
	m_Cells = cellDimensions;
	m_CellSize = { cont.m_Length / cellDimensions.x, cont.m_Height / cellDimensions.y };
	m_Position = cont.m_Position;

	glm::vec3 start = { 
		cont.m_SideD.m_Position.x + (m_CellSize.x / 2),
		cont.m_SideC.m_Position.y + (m_CellSize.y / 2),
		0 };

	m_RenderSquare.update_Size(m_CellSize.x, m_CellSize.y);
	m_RenderSquare.update_Position(start);
	m_RenderSquare.update_Vertices();
}