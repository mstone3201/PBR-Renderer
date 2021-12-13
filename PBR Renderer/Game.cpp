#include "Game.h"
#include "GameMetrics.h"

Game::Game(HINSTANCE hInstance) {
	gameHandle = this;

	WNDCLASSEX wcx{};
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = gameWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcx.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = metrics.gameName;
	wcx.hIconSm = NULL;
	RegisterClassEx(&wcx);

	hwnd = CreateWindow(metrics.gameName, metrics.gameName, WS_OVERLAPPEDWINDOW, 0, 0, metrics.width, metrics.height, NULL, NULL, hInstance, NULL);
}

HRESULT Game::init() {
	HRESULT result = graphics.init(hwnd);
	if(FAILED(result)) return result;
	return scene.init(graphics);
}

int Game::loop() {
	MSG msg{};

	ShowWindow(hwnd, true);
	graphics.setFullScreen();
	timer.start();
	while(running) {
		input.clear();
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		input.update(hwnd);

		float elapsed = timer.elapsedSeconds();
		timer.start();

		if(input.keyDown(KEYBIND::INPUT_MENU)) running = false;

		scene.update(elapsed);

		scene.render(graphics);

		graphics.render();
	}

	graphics.setFullScreen(false);

	return (int)msg.wParam;
}

LRESULT CALLBACK Game::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
	case WM_DESTROY:
	case WM_CLOSE:
	{
		running = false;
		return 0;
	}
	case WM_SIZE:
	{
		graphics.resize();
		return 0;
	}
	case WM_INPUT:
	{
		UINT bufferSize = sizeof(RAWINPUT);
		char buffer[sizeof(RAWINPUT)]{};
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, (LPVOID)buffer, &bufferSize, sizeof(RAWINPUTHEADER));
		RAWINPUT* rawInput = (RAWINPUT*)buffer;
		if(rawInput->header.dwType == RIM_TYPEMOUSE) {
			input.processMouse(rawInput->data.mouse);
		} else if(rawInput->header.dwType == RIM_TYPEKEYBOARD) {
			input.processKeyboard(rawInput->data.keyboard);
		}
		return 0;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK gameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return gameHandle->wndProc(hwnd, message, wParam, lParam);
}