#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include "glm/glm.hpp"

extern glm::vec2 WINDOW_RESOLUTION;

static const uint16_t MaxQuadCount = 10000;
static const uint16_t MaxVertexCount = MaxQuadCount * 4;
static const uint16_t MaxIndexCount = MaxQuadCount * 6;

static const uint16_t MAX_PARTICLES = 5000;

static const bool ENABLE_GRAVITY = true;

#endif