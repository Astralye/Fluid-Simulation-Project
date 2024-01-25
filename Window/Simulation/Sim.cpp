#include "Sim.h"
#include "Particle.h"

#include <iostream>

Sim::Sim() { time = 0; }

float Sim::getTime() { return time; }

void Sim::timeStep() {time += SIMSTEP;}