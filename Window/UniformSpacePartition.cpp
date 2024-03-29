#include "UniformSpacePartition.h"

// We are only using USP and arrays PURELY for comparision, only makes sense that
// Livespan is after comparisons.

// Updating the arrays would be complicated for implementation due to movement of particles and needing to reset values
void UniformSpacePartition::checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont)
{
	/*
		This is run per frame.
		We would assume that the grid arrangment doesn't change per frame.
		Fixed size only when simulation is running.
		If the simulation is paused/ reset, the cell size can change.
	*/

	// 8 bits -> 256 total, or 16x16 grid arrangement
	// 16 bits -> 65,536 total, or 256x256 grid arrangement
	uint16_t n_Cells = m_Cells.x * m_Cells.y;

	// Size of n_TotalCells is n_Cells * sizeof(int)
	n_TotalCells = new int[n_Cells];
	
	// Set index for cell as a number from 0 to n.
	for (int i = 0; i < n_Cells; i++) {
		n_TotalCells[i] = i;
	}

	// If we normalized positions to be only inside the box, we can easily calculate which index it lies at
	// Current positions are in worldspace,

	glm::vec2 xbounds = { cont.m_Position.x - (cont.m_Length / 2) ,cont.m_Position.x + (cont.m_Length / 2) };
	glm::vec2 ybounds = { cont.m_Position.y - (cont.m_Height / 2) ,cont.m_Position.y + (cont.m_Height / 2) };

	for (int i = 0; i < particleArray->size(); i++) {
		glm::vec4 particlePos = particleArray->at(i).m_Position;
		// Bounds checking
		if ((xbounds.x <= particlePos.x && particlePos.x <= xbounds.y) &&
			(ybounds.x <= particlePos.y && particlePos.y <= ybounds.y)) {

			// position - smallbound
			glm::vec2 localPos = { particlePos.x - xbounds.x, particlePos.y - ybounds.x };
			localPos = {
				(float)floor(localPos.x / m_CellSize.x),
				(float)floor(localPos.y / m_CellSize.y)
			};

			std::cout << localPos.x << "," << localPos.y << std::endl;
		}
	}
	// Try not to use a nested loop. It may affect performance.
}

void UniformSpacePartition::Dealloc()
{
	delete[] n_TotalCells;
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