#include "Simulation/Rectangle.h"

// Rectangle Container

void RectangleContainer::update(){
	float thickness = 1;
	// Top and Bottom
	m_SideA.update_Size(m_Length,thickness);
	m_SideA.update_Position({ m_Position.x, m_Position.y + (m_Height / 2), 0.0f});
	m_SideA.update_Vertices();

	m_SideC.update_Size(m_Length, thickness);
	m_SideC.update_Position({ m_Position.x, m_Position.y - (m_Height / 2), 0.0f });
	m_SideC.update_Vertices();

	// Left and Right
	m_SideB.update_Size(thickness, m_Height);
	m_SideB.update_Position({ m_Position.x + (m_Length / 2), m_Position.y, 0.0f});
	m_SideB.update_Vertices();

	m_SideD.update_Size(thickness, m_Height);
	m_SideD.update_Position({ m_Position.x - (m_Length / 2), m_Position.y, 0.0f });
	m_SideD.update_Vertices();
}

// Rectangle
// ---------------------------------------------------------------------------

void Rectangle::update_Position(glm::vec3 pos) {
	m_Position = pos;
}

void Rectangle::add_Position(glm::vec3 pos) {
	m_Position += pos;
}

void Rectangle::update_Size(float length, float height) {
	this->length = length;
	this->height = height;
}

void Rectangle::update_Vertices() {

	m_Vertices[0] =
	{ m_Position.x - (length / 2), m_Position.y + (height / 2) }; // TL

	m_Vertices[1] =
	{ m_Position.x + (length / 2), m_Position.y + (height / 2) }; // TR

	m_Vertices[2] =
	{ m_Position.x + (length / 2), m_Position.y - (height / 2) }; // BR

	m_Vertices[3] =
	{ m_Position.x - (length / 2), m_Position.y - (height / 2) }; // BL
}