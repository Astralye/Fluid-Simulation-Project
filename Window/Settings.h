#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include "glm/glm.hpp"

struct Settings {
	constexpr static float SIMSTEP = 0.005f; // An integer is one second.

	// 16 : 9
	constexpr static glm::vec2 WINDOW_RESOLUTION{ 1280.0f, 720.0f };

	// 800 x 800 = 1:1 ratio
	// 1280 x 720 = 16:9 ratio

	static const uint16_t MaxQuadCount = 10000;
	static const uint16_t MaxVertexCount = MaxQuadCount * 4;
	static const uint16_t MaxIndexCount = MaxQuadCount * 6;

	static const uint16_t MAX_PARTICLES = 500;

	static bool ENABLE_GRAVITY;
	static bool ENABLE_DEBUG_MODE;
};

#endif