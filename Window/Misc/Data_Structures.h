#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "glm/glm.hpp"

struct Vertex {
	glm::vec4 WorldPosition;
	glm::vec4 Colour;
};

struct CircleVertex {
	glm::vec4 WorldPosition;
	glm::vec4 LocalPosition;
	glm::vec4 Colour{ 1.0f, 1.0f, 1.0f, 1.0f };
};

enum VertexType {
	Null,
	QUAD,
	CIRCLE
};

#endif