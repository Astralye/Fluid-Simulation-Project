#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "glm/glm.hpp"

// Possibly change the name to quad
class Rectangle {
public:
	glm::vec3 m_Position;
	glm::vec2 m_Vertices[4];
	glm::vec3 m_Colour;

	float length;
	float height;

	Rectangle(glm::vec3 pos = { 0.0f, 0.0f, 0.0f },
		float length = 1,
		float height = 1,
		glm::vec3 colour = { 0.2f, 0.2f, 0.2f}
	)
		: m_Position(pos),
		length(length),
		height(height),
		m_Colour(colour)
	{
		update_Vertices();
	}

	void update_Size(float length, float height);
	void update_Position(glm::vec3 pos);
	void add_Position(glm::vec3 pos);
	void update_Vertices();
};


class RectangleContainer {
public:

	// Contains 4 Rectangle objects
	Rectangle m_SideA; // Top
	Rectangle m_SideB; // Right
	Rectangle m_SideC; // Bottom
	Rectangle m_SideD; // Left

	float m_Length;
	float m_Height;

	glm::vec3 m_Position;

	RectangleContainer(glm::vec3 pos = { 0.0f,0.0f,0.0f },
		float length = 1,
		float height = 1,
		float thickness = 1
	)
		: m_Position(pos),
		m_Height(height),
		m_Length(length),
		m_SideA(glm::vec3{ pos.x			   , pos.y + (height / 2), 0.0f }, length, thickness),
		m_SideB(glm::vec3{ pos.x + (length / 2), pos.y				 , 0.0f }, thickness, height),
		m_SideC(glm::vec3{ pos.x			   , pos.y - (height / 2), 0.0f }, length, thickness),
		m_SideD(glm::vec3{ pos.x - (length / 2), pos.y				 , 0.0f }, thickness, height)
	{}

	void update();
};


#endif