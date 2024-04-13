#include "Particle.h"

// Collision functions
// --------------------------------------------------------------------------------------------------

void Collision::collisionResponse(Particle& A, collisionType::Type type) {

	glm::vec3 flip{ false,false,false };

	switch (type) {
	case collisionType::Horizontal:

		if (Settings::ENABLE_GRAVITY) {

			if (A.getVelocity().y != 0) {
				A.m_Position.y += 0.05;
			}

			A.bounce();
		}
		else {
			flip.y = true;
		}
		break;

	case collisionType::Vertical:
		flip.x = true;
		break;
	}

	A.invert(flip);
}

bool Collision::collisionDetection(Particle& A, Particle& B) {
	float distance = PhysicsEq::euclid_Distance(A.m_Position, B.m_Position);
	auto maxLength = A.getRadius() + B.getRadius();

	if (distance <= maxLength) {
		return true;
	}
	return false;
}

bool Collision::collisionDetection(Rectangle& A, Particle& B) {

	glm::vec4 clamp;

	clamp.x = PhysicsEq::clamp(B.m_Position.x, A.m_Position.x - (A.length / 2), A.m_Position.x + (A.length / 2));
	clamp.y = PhysicsEq::clamp(B.m_Position.y, A.m_Position.y - (A.height / 2), A.m_Position.y + (A.height / 2));

	glm::vec4 difference = clamp - B.m_Position;

	float overlap = B.getRadius() - PhysicsEq::pythagoras({ difference.x, difference.y });

	if (std::isnan(overlap)) {
		overlap = 0;
	}

	if (overlap > 0) {
		return true;
	}
	return false;
}

// Collision between a Rectangle and Particle
collisionType Collision::collisionDetection(RectangleContainer& A, Particle& B) {
	if (collisionDetection(A.m_SideA, B) || collisionDetection(A.m_SideC, B)) {
		return { true, collisionType::Horizontal };
	}

	if (collisionDetection(A.m_SideB, B) || collisionDetection(A.m_SideD, B)) {
		return { true, collisionType::Vertical };
	}

	return { false , collisionType::N_A };
}

// Particle
// ---------------------------------------------------------------------------------------------------


// Static variables
// -------------------------------------------------------------------------------------------------------
float Particle::KERNEL_RADIUS = 0.0f;
float Particle::particleProperties[Settings::MAX_PARTICLES] = { 0 };
Particle::DebugType Particle::Debug = DebugType::D_Velocity;

// Static functions
// ---------------------------------------------------------------------------------------------------

// Particle Initializers
void Particle::init_Cube(std::vector<Particle> *particleArray, float radius, float spacing, uint16_t nParticles)
{
	Particle::KERNEL_RADIUS = 4 * radius;

	// 8 bit -> 256^2 = max 65k particles
	uint16_t column, row;
	float squareDimension;

	glm::vec2 offset, position, max_Size;
	glm::vec2 containerCenter = { -50.0f,50.0f };

	squareDimension = (float)sqrt(Settings::MAX_PARTICLES);

	offset = { containerCenter.x / 2, containerCenter.y / 2 };
	max_Size = { (int)ceil(squareDimension), (int)floor(squareDimension) };

	// Set base values
	column = 0;
	row = 0;

	for (int i = 0; i < Settings::MAX_PARTICLES; i++) {

		if (i >= nParticles) { return; }

		position = { row * (2 * radius + spacing),
					column * (2 * radius + spacing)};
		row++;

		if (row == max_Size.x) {
			column++;
			row = 0;
		}
		particleArray->emplace_back(
			glm::vec4(position.x + offset.x, position.y + offset.y, 0.0f, 0.0f), 1.0f, radius);
	}
}
void Particle::init_Random(std::vector<Particle> *particleArray, float radius , uint16_t currentNumberParticles) {


	Particle::KERNEL_RADIUS = 8 * radius;

	int maxVelocity = 20;

	for (int i = 0; i < currentNumberParticles; i++) {

		particleArray->emplace_back(
			glm::vec4(8.0f + rand() % 85, 12.0f + rand() % 70, 0.0f,0.0f), 1.0f, radius
			//glm::vec3((rand() % maxVelocity), (rand() % maxVelocity), 0.0f),
		);
	}
}

void Particle::SetPredictedPosition() {
	glm::vec3 pos{ m_Position.x, m_Position.y, m_Position.z };

	pos.x += m_PredictedVelocity.x * Settings::SIMSTEP;
	pos.y += m_PredictedVelocity.y * Settings::SIMSTEP;
	pos.z += m_PredictedVelocity.z * Settings::SIMSTEP;


	m_PredictedPos = glm::vec4(pos, 0.0f);
}

void Particle::update(){

	glm::vec3 pos{ m_Position.x, m_Position.y, m_Position.z };

	pos.x += m_Velocity.x * Settings::SIMSTEP;
	pos.y += m_Velocity.y * Settings::SIMSTEP;
	pos.z += m_Velocity.z * Settings::SIMSTEP;

	m_Position = glm::vec4(pos, 0.0f);

	// Updates vertices
	m_Vertices[0] =
		{ m_Position.x - m_Radius, m_Position.y + m_Radius }; // TL

	m_Vertices[1] =
		{ m_Position.x + m_Radius, m_Position.y + m_Radius }; // TR

	m_Vertices[2] =
		{ m_Position.x + m_Radius, m_Position.y - m_Radius }; // BR

	m_Vertices[3] =
		{ m_Position.x - m_Radius, m_Position.y - m_Radius }; // BL

}

void Particle::bounce() {
	float bounceCoEff = 0.7f;

	m_Acceleration.y = invert(m_Acceleration.y) * ((float)rand()) / RAND_MAX;
	m_Velocity.y = bounceCoEff * invert(m_Velocity.y) * (((float)rand()) / RAND_MAX) ;

	m_Velocity.x = m_Velocity.x * 0.7f;
}

// Inverts value
float Particle::invert(float value) { return (value < 0) ? abs(value) : -value; }

void Particle::invert(Vector type)
{
	if (type == Vector::V_Acceleration) {
		m_Acceleration.x = -m_Acceleration.x;
	}else
	{
		m_Velocity.x = -m_Velocity.x;
	}
}

void Particle::invert(glm::vec3 type)
{
	if (type.x) {
		m_Velocity.x = -m_Velocity.x;
		m_Acceleration.x = -m_Acceleration.x;
	}

	if (type.y) {
		m_Velocity.y = -m_Velocity.y;
		m_Acceleration.y = -m_Acceleration.y;
	}

	if (type.z) {
		m_Velocity.z = -m_Velocity.z;
		m_Acceleration.z = -m_Acceleration.z;
	}
}

glm::vec4 Particle::DebugColour()
{
	glm::vec3 maxColour = { 1.0f, 0.03f, 0.03f };
	glm::vec3 minColour = { 0.0f, 0.3f , 1.0f };

	float max, min, value, percentage;

	if (Debug == DebugType::D_Velocity) {
		// Arbituary values
		max = 125.0f;
		min = 0.0f;

		value = PhysicsEq::pythagoras(m_Velocity);
	}

	if (Debug == DebugType::D_Density) {
		max = 9 * PhysicsEq::REST_DENSITY;
		min = 0.0f;

		value = m_Density;
	}

	if (Debug == DebugType::D_Pressure) {
		max = 500.0f;
		min = 0.0f;

		value = PhysicsEq::pythagoras(m_Pressure);
	}

	percentage = (value - min) / (max - min);

	glm::vec3 newColour;
	newColour.x = PhysicsEq::lerp(minColour.x, maxColour.x, percentage);
	newColour.y = PhysicsEq::lerp(minColour.y, maxColour.y, percentage);
	newColour.z = PhysicsEq::lerp(minColour.z, maxColour.z, percentage);

	return glm::vec4(newColour,1.0f);
}

void Particle::setRadius(float rad) { m_Radius = rad; }

void Particle::setVelocity(glm::vec2 vel) {
	m_Velocity.x = vel.x;
	m_Velocity.y = vel.y;
}

void Particle::setPredictedVelocity(glm::vec3 pVel) { m_PredictedVelocity = pVel; }

void Particle::setAcceleration(glm::vec2 acc) { m_Acceleration = glm::vec3(acc, 0); }

void Particle::addVelocity(glm::vec2 vel) {
	m_Velocity.x += vel.x;
	m_Velocity.y += vel.y;
}

void Particle::setDensity(float den) { m_Density = den; }
void Particle::setPressure(glm::vec2 pressure) { m_Pressure = pressure; }
