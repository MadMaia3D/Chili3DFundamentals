#pragma once
#include <chrono>

class FrameTimer {
public:
	FrameTimer() {
		lastMarkedTime = std::chrono::steady_clock::now();
	}

	float Mark() {
		auto currentTime = std::chrono::steady_clock::now();

		std::chrono::duration<float> elapsedTime = currentTime - lastMarkedTime;
		lastMarkedTime = currentTime;
		float elapsedSeconds = elapsedTime.count();
		return elapsedSeconds;
	}
private:
	std::chrono::steady_clock::time_point lastMarkedTime;
};