#include "Statistics.h"

Statistics stats;
Timer timer;

// Couldn't use macro, using function pointers

// member function pointer
void TIME(void (test::Test::*func)(), test::Test* a, std::chrono::duration<float>& timer){
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	(a->*func)();
	end = std::chrono::system_clock::now();
	timer = end - start;
}

// Timer for function with particle array parameter
void TIME(void (*func)(std::vector<Particle>*), std::vector<Particle>* particleArray, std::chrono::duration<float>& timer) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	func(particleArray);
	end = std::chrono::system_clock::now();
	timer = end - start;
}

// Timer for function with particle array and container parameter
void TIME(void (*func)(std::vector<Particle>*, RectangleContainer& container), std::vector<Particle>* particleArray, RectangleContainer& container, std::chrono::duration<float>& timer) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	func(particleArray, container);
	end = std::chrono::system_clock::now();
	timer = end - start;
}

void Timer::startTimer()
{
	start = std::chrono::system_clock::now();
}

void Timer::endTimer(std::chrono::duration<float>& timer) {
	end = std::chrono::system_clock::now();
	timer = end - start;
}
