#pragma once

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

	void dataToCSV(float data);

public:
	std::vector<float> *data;

	Benchmark(float interval = 0.033f, int duration = 20)
		: startTime(std::chrono::system_clock::now()), interval(interval), endTime(duration), timeElapsed(0)
	{
		data = new std::vector<float>;

		// estimation for space
		float spaces = ((1 / interval) * duration);
		std::cout << spaces << std::endl;
		data->reserve((int)spaces);
	};

	~Benchmark() {
		delete data;
	}

	void run(float FPS);
	bool isStillBenchmark();

};