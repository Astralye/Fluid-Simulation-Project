#pragma once

#include "glm/glm.hpp"

// UL, upper left, UR, upper right etc
struct RectCoords {
	glm::vec2 m_TopLeft;
	glm::vec2 m_TopRight;
	glm::vec2 m_BottomLeft;
	glm::vec2 m_BottomRight;

	// Constructor for struct
	RectCoords(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
		: m_TopLeft(a), m_TopRight(b), m_BottomLeft(c), m_BottomRight(d)
	{
	}
};

class Rectangle {
public:
	glm::vec3 m_Position;
	RectCoords m_Coords;

	float length;
	float height;

	Rectangle(glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
		float length = 1,
		float height = 1
	)
		: m_Position(pos),
		length(length),
		height(height),
		m_Coords(
			glm::vec2(m_Position.x - (length / 2), m_Position.y + (height / 2)), // TL
			glm::vec2(m_Position.x + (length / 2), m_Position.y + (height / 2)), // TR
			glm::vec2(m_Position.x - (length / 2), m_Position.y - (height / 2)), // BL
			glm::vec2(m_Position.x + (length / 2), m_Position.y - (height / 2))  // BR
		)
	{
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
