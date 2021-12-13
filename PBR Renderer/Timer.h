#pragma once

#include <Windows.h>

class Timer {
	INT64 frequency = 0, startTime = 0;
public:
	Timer();
	void start();
	INT64 elapsed();
	float elapsedSeconds();
};