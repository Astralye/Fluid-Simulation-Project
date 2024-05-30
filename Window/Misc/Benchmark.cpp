#include "Benchmark.h"

bool Benchmark::checkInterval()
{

	float timeAtPreviousFrame, timeCurrent, timeSincePreviousFrame;
	std::chrono::duration<float> timeElapsedLocal;

	// The current time was calculated the previous frame.
	timeAtPreviousFrame = timeElapsed.count();
	currentTime = std::chrono::system_clock::now();

	timeElapsedLocal = currentTime - startTime;
	timeCurrent = timeElapsedLocal.count();

	timeSincePreviousFrame = timeCurrent - timeAtPreviousFrame;

	if (timeSincePreviousFrame >= interval) {
		timeElapsed = timeElapsedLocal;
		return true;
	}
	
	return false;
}

bool Benchmark::isStillBenchmark()
{
	return (timeElapsed.count() < endTime.count()) ? true : false;
}

void Benchmark::run(float FPS, glm::vec4 msData)
{
	if (checkInterval()) {
		data->emplace_back(FPS);
		msTimers->emplace_back(msData);
	}

	if (!isStillBenchmark()) {
		float avg = std::accumulate(data->begin(), data->end(), 0);
		avg /= data->size();

		std::cout << "Average FPS:" << avg << " n: "<< data->size() <<std::endl;

		glm::vec4 tmp = {0,0,0,0};

		for (int i = 0; i < msTimers->size(); i++) {
			tmp.x += msData.x;
			tmp.y += msData.y;
			tmp.z += msData.z;
			tmp.w += msData.w;
		}

		tmp /= msTimers->size();

		dataToCSV(avg, tmp);

		// Perhaps display to user that results are stored in X location.
	}
}

void Benchmark::dataToCSV(float data, glm::vec4 msTimes) {

	auto time = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(time);

	struct tm newTime;

	localtime_s(&newTime, &in_time_t);

	std::stringstream ss;
	ss << "Benchmark " << (newTime.tm_year + 1900) << "-" << newTime.tm_mon << "-" << newTime.tm_mday << "-" << newTime.tm_hour << "-" << newTime.tm_min << "-" << newTime.tm_sec << ".csv";
	auto str = ss.str();

	std::filesystem::path parent = std::filesystem::current_path().parent_path();

	// Creates directory
	std::filesystem::create_directory(parent / "Benchmark");

	std::ofstream outfile(parent / "Benchmark" / str);

	outfile << "Average" << "," << "Framerate (FPS)" << ", "
		<< "Time create Lookup Table (ms)" << "," 
		<< "Time sort Lookup (ms)" << "," 
		<< "Time Check Neighbours (ms)" << "," 
		<< "Render Time (ms)" << "\n";
	outfile << " ," << data << "," << msTimes.x << "," << msTimes.y << "," << msTimes.z << "," << msTimes.w;
	outfile.close();
}