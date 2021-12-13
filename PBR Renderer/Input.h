#pragma once

#include <Windows.h>

enum class KEYBIND {
	INPUT_MENU,
	INPUT_ROTATE_LEFT,
	INPUT_ROTATE_RIGHT,
	NUM_INPUT
};

class Input {
	RAWINPUTDEVICE rid[2];
	USHORT keymap[(int)KEYBIND::NUM_INPUT];
	bool input[(int)KEYBIND::NUM_INPUT][2]{}, keys[256]{};
	float x = 0.0f, y = 0.0f, dx = 0.0f, dy = 0.0f, scroll = 0.0f;
public:
	Input();
	void processMouse(RAWMOUSE), processKeyboard(RAWKEYBOARD), update(HWND), clear();
	bool isInput(KEYBIND), keyDown(KEYBIND), keyUp(KEYBIND);
	float getX(), getY(), getDX(), getDY(), getScroll();
};