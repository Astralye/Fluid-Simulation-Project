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
	//std::cout << "index sizes :" << startIndex->size() << std::endl;
	//timer.startTimer();
	// This process takes the longest
	// I could try and use threads to complete these quicker? 

	// Sum over all cells
	//float sumDensity = 0;
	//float sumPressure = 0;
	//float sumViscosity = 0;
	//float sumMove = 0;

	//float sum = 0;
	omp_set_num_threads(100);
	#pragma omp parallel for
	// For every filled partition
	for (int i = 0; i < startIndex->size(); i++) {
		int particlestartIndex = startIndex->at(i);
		if (particlestartIndex == std::numeric_limits<int>().max()) continue;

		int particleCellKey = lookupList->at(particlestartIndex).cellKey;

		// Per cell
		neighbourCells(particleArray, particleCellKey, cont);
		//sum += stats.Time_Neighbour_Cells.count();

		//sumDensity += stats.Time_Calculate_Density.count();
		//sumPressure += stats.Time_Calculate_Pressure.count();
		//sumViscosity += stats.Time_Calculate_Viscosity.count();
		//sumMove += stats.Time_Calculate_Movement.count();
	}
	//timer.endTimer(stats.Time_Neighbour_Cells);
	//std::cout << "neighbour cell init:" << sum * 1000 << "\n" <<
	//	"density:" << sumDensity * 1000 << "\n" <<
	//	"pressure:" << sumPressure * 1000 << "\n" <<
	//	"viscosity:" << sumViscosity * 1000 << "\n" <<
	//	"bounds check:" << sumMove * 1000 << "\n" <<
	//	"total time:" << (sum + sumDensity + sumPressure + sumViscosity + sumMove) * 1000  << "\n" << std::endl;
	//
}


// Coordinates in the container
void UniformSpacePartition::neighbourCells(std::vector<Particle>* particleArray, int index , RectangleContainer& cont)
{
	timer.startTimer();
	int Offset[3] = { -1, 0, 1 };

	// Convert the index to its coordinate
	glm::vec2 BaseCoordinate = indexToCoord(index);
	glm::vec2 newCoord[9];

	int neighbourCellKey;
	int currentIndex;

	// Might need to set these values to a static value.

	// Changing this to a large enough static value may improve performance
	
	// Stores all the possible particles for collisions
	std::vector<int> particleIndices;
	// In any event, wouldn't expect this to ever surpass 10-15 particles.


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

	//if (particleIndices.size() > 20) {
	//	std::cout << "SIZE BIGGER" << std::endl;
	//}

	timer.endTimer(stats.Time_Neighbour_Cells);
	// This code above has a constant of 4-5ms regardless of particle array count.

	SPH::CalculateAllDensities(particleArray, particlesInMainCell, particleIndices);
	SPH::CalculateAllPressures(particleArray, particlesInMainCell, particleIndices);

	SPH::CalculatePositionCollision(particleArray, particlesInMainCell, cont, isBorderCell(BaseCoordinate));
	// Check out of bounds

	SPH::CalculateAllViscosities(particleArray, particlesInMainCell, particleIndices);

	// This is where the code starts to slow down.
	//timer.startTimer();
	//SPH::CalculateAllDensities(particleArray, particlesInMainCell, particleIndices);
	//timer.endTimer(stats.Time_Calculate_Density);

	////std::cout << stats.Time_Calculate_Density.count() * 1000 << std::endl;

	//timer.startTimer();
	//SPH::CalculateAllPressures(particleArray, particlesInMainCell, particleIndices);
	//timer.endTimer(stats.Time_Calculate_Pressure);
	//
	//timer.startTimer();
	//// This code could just check for cells close to the border. Otherwise, no need to run the function.
	//SPH::CalculatePositionCollision(particleArray, particlesInMainCell, cont);
	//timer.endTimer(stats.Time_Calculate_Movement);

	//timer.startTimer();
	//SPH::CalculateAllViscosities(particleArray, particlesInMainCell, particleIndices);
	//timer.endTimer(stats.Time_Calculate_Viscosity);
	//std::cout << "Changing cell \n" << std::endl;
}

// Checks if the index is by the container border
bool UniformSpacePartition::isBorderCell(glm::vec2 coordinate)
{
	if (coordinate.x == 0 || coordinate.x == m_Cells.x-1 || coordinate.y == 0 || coordinate.y == m_Cells.y-1)
	{
		return true;
	}
	return false;
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