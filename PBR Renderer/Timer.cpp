#include "Timer.h"

Timer::Timer() {
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
}

void Timer::start() {
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

INT64 Timer::elapsed() {
	INT64 current = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	return current-startTime;
}

float Timer::elapsedSeconds() {
	return (float)elapsed()/frequency;
}