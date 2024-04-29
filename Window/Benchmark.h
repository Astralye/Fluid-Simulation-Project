#pragma once

#include "glm/glm.hpp"

#include <vector>
#include <chrono>
#include <numeric>

#include <fstream>

#include <ctime>

#include <sstream>
#include <iostream>
#include <iomanip>

#include <filesystem>

class Benchmark {

private:

	std::chrono::time_point<std::chrono::system_clock> startTime, currentTime;
	std::chrono::seconds endTime;
	std::chrono::duration<float> timeElapsed;
	
	// in seconds
	float interval;

	bool checkInterval();

	void dataToCSV(float data, glm::vec4 msTimes);

public:
	std::vector<float> *data;
	std::vector<glm::vec4> *msTimers;

	Benchmark(float interval = 0.033f, int duration = 20)
		: startTime(std::chrono::system_clock::now()), interval(interval), endTime(duration), timeElapsed(0)
	{
		data = new std::vector<float>;
		msTimers = new std::vector<glm::vec4>;

		// estimation for space
		float spaces = ((1 / interval) * duration);
		std::cout << spaces << std::endl;
		data->reserve((int)spaces);
		msTimers->reserve((int)spaces);
	};

	~Benchmark() {
		delete data;
		delete msTimers;
	}

	void run(float FPS, glm::vec4 msData);
	bool isStillBenchmark();

};