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

	// uint16 can store up to 65k values. Can change to 32 if needed.
	static const int MAX_PARTICLES = 20000;
	static int CURRENT_PARTICLES;

	static bool ENABLE_GRAVITY;
	static bool ENABLE_DEBUG_MODE;
	static bool ENABLE_PARTITION_BACKROUND;
	static bool ENABLE_HOVER_PARTICLE;
	static bool ENABLE_RESIZE_CONTAINER;
	static bool ENABLE_SQUARE_PARTITIONS;
	static bool ENABLE_SOURCE;
	static bool INIT_SIM;

	static bool START_BENCHMARK;
	static bool CREATE_BENCHMARK;

	static bool PAUSE_SIMULATION;
	static bool IS_HOVER_PARTICLE;
	static bool OPEN_POPUP;

	static int HOVER_PARTICLE;
	static int CLICK_PARTICLE;
	static int PARITIONS_SIZE;
};

#endif