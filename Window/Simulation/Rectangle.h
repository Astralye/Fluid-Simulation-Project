#pragma once

#include "glm/glm.hpp"

// Possibly change the name to quad
class Rectangle {
public:
	glm::vec3 m_Position;
	glm::vec2 m_Vertices[4];

	float length;
	float height;

	Rectangle(glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
		float length = 1,
		float height = 1
	)
		: m_Position(pos),
		length(length),
		height(height)
	{
		m_Vertices[0] =
			{ m_Position.x - (length / 2), m_Position.y + (height / 2) }; // TL

		m_Vertices[1] =
			{ m_Position.x + (length / 2), m_Position.y + (height / 2) }; // TR
			
		m_Vertices[2] =
			{ m_Position.x + (length / 2), m_Position.y - (height / 2) }; // BR

		m_Vertices[3] =
			{ m_Position.x - (length / 2), m_Position.y - (height / 2) }; // BL
	}
};


class RectangleContainer {
public:

	// Contains 4 Rectangle objects
	Rectangle m_SideA; // Top
	Rectangle m_SideB; // Right
	Rectangle m_SideC; // Bottom
	Rectangle m_SideD; // Left

	glm::vec3 m_Position;

	RectangleContainer(glm::vec3 pos = { 0.0f,0.0f,0.0f },
		float length = 1,
		float height = 1,
		float thickness = 1
	)
		: m_Position(pos),
		m_SideA(glm::vec3{ pos.x			   , pos.y + (height / 2), 0.0f }, length, thickness),
		m_SideB(glm::vec3{ pos.x + (length / 2), pos.y				 , 0.0f }, thickness, height),
		m_SideC(glm::vec3{ pos.x			   , pos.y - (height / 2), 0.0f }, length, thickness),
		m_SideD(glm::vec3{ pos.x - (length / 2), pos.y				 , 0.0f }, thickness, height)
	{
	}
};
