#pragma once

#include "Timer.h"
#include "Input.h"
#include "Graphics.h"
#include "Scene.h"

class Game {
	HWND hwnd;
	bool running = true;
	Timer timer;
	Input input;
	Graphics graphics;
	Scene scene;
public:
	Game(HINSTANCE);
	HRESULT init();
	int loop();
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
};

static Game* gameHandle = 0;

LRESULT CALLBACK gameWndProc(HWND, UINT, WPARAM, LPARAM);