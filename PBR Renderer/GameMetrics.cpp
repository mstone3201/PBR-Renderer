#include "GameMetrics.h"

GameMetrics metrics;

GameMetrics::GameMetrics() {
	width = GetSystemMetrics(SM_CXFULLSCREEN);
	height = GetSystemMetrics(SM_CYFULLSCREEN);
}