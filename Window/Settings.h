#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include "glm/glm.hpp"

constexpr static float SIMSTEP = 0.005f; // An integer is one second.

constexpr static glm::vec2 WINDOW_RESOLUTION { 800.0f,800.0f };

static const uint16_t MaxQuadCount = 10000;
static const uint16_t MaxVertexCount = MaxQuadCount * 4;
static const uint16_t MaxIndexCount = MaxQuadCount * 6;

static const uint16_t MAX_PARTICLES = 100;

static const bool ENABLE_GRAVITY = false;

#endif