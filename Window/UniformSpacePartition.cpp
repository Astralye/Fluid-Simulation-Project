#include "UniformSpacePartition.h"

int UniformSpacePartition::coordToIndex(glm::vec2 coord)
{
	int cellIndex = coord.y * m_Cells.x;
	cellIndex += coord.x;

	return cellIndex;
}

glm::vec2 UniformSpacePartition::indexToCoord(int index)
{
	int yCoord = 0;
	while (index >= m_Cells.y) {
		index -= m_Cells.y;
		yCoord += 1;
	}

	return glm::vec2(index, yCoord);
}

// Updating the arrays would be complicated for implementation due to movement of particles and needing to reset values
void UniformSpacePartition::checkPartition(std::vector<Particle>* particleArray, RectangleContainer& cont)
{

	timer.startTimer();

	// Sets all the values back to 0.
	lookupList->clear();

	// Clears indices and fills it with a null value
	startIndex->clear();
	startIndex->resize( (m_Cells.x * m_Cells.y) );
	std::fill(startIndex->begin(), startIndex->end(), std::numeric_limits<int>().max());

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
			glm::vec2 localPos = { 
				particlePos.x - xbounds.x,
				ybounds.y - particlePos.y
			};

			localPos = {
				(float)floor(localPos.x / m_CellSize.x),
				(float)floor(localPos.y / m_CellSize.y)
			};

			lookupList->emplace_back(coordToIndex(localPos),i);
		}
	}

	timer.endTimer(stats.Time_Create_Lookup);

	timer.startTimer();
	// Sorts by cellID
	std::sort(lookupList->begin(), lookupList->end(), compareCellID);
	timer.endTimer(stats.Time_Sort_Lookup);


	// Stores the first occurance of a new cell position
	// Null / max value if the same as before.
	for (int i = 0; i < lookupList->size(); i++) {

		int key = lookupList->at(i).cellKey;
		int	previousKey = (i == 0) ? std::numeric_limits<int>().max() : lookupList->at(i - 1).cellKey;

		if (key != previousKey) {
			startIndex->at(key) = i;
		}

	}
}

void UniformSpacePartition::getNeighbourParticles(std::vector<Particle>* particleArray, RectangleContainer& cont)
{
	timer.startTimer();
	// This process takes the longest
	// I could try and use threads to complete these quicker? 

	omp_set_num_threads(20);
	#pragma omp parallel for
	// For every filled partition
	for (int i = 0; i < startIndex->size(); i++) {
		int particlestartIndex = startIndex->at(i);
		if (particlestartIndex == std::numeric_limits<int>().max()) continue;

		int particleCellKey = lookupList->at(particlestartIndex).cellKey;
		neighbourCells(particleArray, particleCellKey, cont);
	}
	timer.endTimer(stats.Time_Neighbour_Cells);
}


// Coordinates in the container
void UniformSpacePartition::neighbourCells(std::vector<Particle>* particleArray, int index , RectangleContainer& cont)
{
	int Offset[3] = { -1, 0, 1 };

	// Convert the index to its coordinate
	glm::vec2 BaseCoordinate = indexToCoord(index);
	glm::vec2 newCoord[9];

	int neighbourCellKey;
	int currentIndex;

	// Might need to set these values to a static value.

	// Stores all the possible particles for collisions
	std::vector<int> particleIndices;

	// All the particles that can be compared
	std::vector<int> particlesInMainCell;

	// Creates coordinates
	int n = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			newCoord[n] = {
				BaseCoordinate.x + Offset[j],
				BaseCoordinate.y + Offset[i]
			};

			n++;
		}
	}

	// For each quadrant in the 3x3 grid
	for (int i = 0; i < 9; i++) {
		
		// Skips quadrants with invalid cell coordinates
		if (!(	(0 <= newCoord[i].x && newCoord[i].x <= m_Cells.x - 1) &&
				(0 <= newCoord[i].y && newCoord[i].y <= m_Cells.y - 1))) continue;

		neighbourCellKey = coordToIndex(newCoord[i]);
		currentIndex = startIndex->at(neighbourCellKey);

		// Skips quadrants with null index
		if (currentIndex == std::numeric_limits<int>().max()) { continue; }

		// For every particle in the quadrant
		for (int j = currentIndex; j < lookupList->size(); j++) {
			// Exit if it has a different key
			if (lookupList->at(j).cellKey != neighbourCellKey) break;

			// Particles in the center cell. This is what is used for comparisons
			if (i == 4) { particlesInMainCell.emplace_back(lookupList->at(j).particleIndex); }

			// All particles in the 3x3 grid
			particleIndices.emplace_back(lookupList->at(j).particleIndex);
		}
	}

	//std::cout << particlesInMainCell.size() << std::endl;
	//std::cout << "cell index:" << index << std::endl;


	SPH::CalculateAllDensities(particleArray, particlesInMainCell, particleIndices);
	SPH::CalculateAllPressures(particleArray, particlesInMainCell, particleIndices);
	SPH::CalculatePositionCollision(particleArray, particlesInMainCell, cont);
	SPH::CalculateAllViscosities(particleArray, particlesInMainCell, particleIndices);

	//std::cout << "Changing cell \n" << std::endl;
}

bool UniformSpacePartition::compareCellID(const spatialLookup&a, const spatialLookup&b)
{
	return a.cellKey < b.cellKey;
}

void UniformSpacePartition::InitializeLookup()
{
	lookupList = new std::vector<spatialLookup>;
	startIndex = new std::vector<int>;
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