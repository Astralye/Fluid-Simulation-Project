#include "Settings.h"

bool Settings::ENABLE_GRAVITY = true;
bool Settings::ENABLE_DEBUG_MODE = true; // change back to false
bool Settings::ENABLE_PARTITION_BACKROUND = true; // change back to false
bool Settings::ENABLE_RESIZE_CONTAINER = false;
bool Settings::ENABLE_SQUARE_PARTITIONS = false;
bool Settings::ENABLE_HOVER_PARTICLE = false;
bool Settings::ENABLE_SOURCE = false;

bool Settings::START_BENCHMARK = false;
bool Settings::CREATE_BENCHMARK = false;

bool Settings::PAUSE_SIMULATION = true;
bool Settings::IS_HOVER_PARTICLE = false;
bool Settings::OPEN_POPUP = false;
bool Settings::INIT_SIM = true;

int Settings::CURRENT_PARTICLES = 0;
int Settings::HOVER_PARTICLE = -1;
int Settings::CLICK_PARTICLE = -1;
int Settings::PARITIONS_SIZE = 2;