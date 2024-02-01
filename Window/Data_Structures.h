#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 WorldPosition;
	glm::vec4 Colour;
};

struct CircleVertex {
	glm::vec3 WorldPosition;
	glm::vec3 LocalPosition;
	glm::vec4 Colour{ 1.0f, 1.0f, 1.0f, 1.0f };
};

enum VertexType {
	QUAD,
	CIRCLE
};

#endif