#include "Input.h"
#include "GameMetrics.h"

Input::Input() {
	rid[0].usUsagePage = 1;
	rid[0].usUsage = 2;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = 0;

	rid[1].usUsagePage = 1;
	rid[1].usUsage = 6;
	rid[1].dwFlags = 0;
	rid[1].hwndTarget = 0;

	RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));

	keymap[(unsigned)KEYBIND::INPUT_MENU] = VK_ESCAPE;
	keymap[(unsigned)KEYBIND::INPUT_ROTATE] = VK_LBUTTON;
	keymap[(unsigned)KEYBIND::INPUT_PAN] = VK_RBUTTON;
}

void Input::processMouse(RAWMOUSE mouse) {
	if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) keys[VK_LBUTTON] = true;
	if(mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) keys[VK_LBUTTON] = false;
	if(mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) keys[VK_MBUTTON] = true;
	if(mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) keys[VK_MBUTTON] = false;
	if(mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) keys[VK_RBUTTON] = true;
	if(mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) keys[VK_RBUTTON] = false;
	if(mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) keys[VK_XBUTTON1] = true;
	if(mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) keys[VK_XBUTTON1] = false;
	if(mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) keys[VK_XBUTTON2] = true;
	if(mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) keys[VK_XBUTTON2] = false;
	if(mouse.usButtonFlags & RI_MOUSE_WHEEL) scroll = (float)(SHORT)mouse.usButtonData/WHEEL_DELTA;
	dx = (float)mouse.lLastX/GetSystemMetrics(SM_CYFULLSCREEN)*metrics.mouseSensitivity;
	dy = (float)mouse.lLastY/GetSystemMetrics(SM_CYFULLSCREEN)*metrics.mouseSensitivity;
}

void Input::processKeyboard(RAWKEYBOARD keyboard) {
	if(keyboard.Flags & RI_KEY_BREAK) keys[keyboard.VKey] = false;
	else keys[keyboard.VKey] = true;
}

void Input::update(HWND hwnd) {
	for(unsigned i = 0; i < (unsigned)KEYBIND::NUM_INPUT; i++) {
		input[i][1] = input[i][0];
		input[i][0] = keys[keymap[i]];
	}

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hwnd, &mouse);
	RECT screen;
	if(GetClientRect(hwnd, &screen)) {
		x = (float)mouse.x/(screen.right-screen.left);
		y = (float)mouse.y/(screen.bottom-screen.top);
	}
}

void Input::clear() {
	dx = dy = scroll = 0.0f;
}

bool Input::isInput(KEYBIND id) {
	return input[(unsigned)id][0];
}

bool Input::keyDown(KEYBIND id) {
	return !input[(unsigned)id][1] && input[(unsigned)id][0];
}

bool Input::keyUp(KEYBIND id) {
	return input[(unsigned)id][1] && !input[(unsigned)id][0];
}

float Input::getX() {
	return x;
}

float Input::getY() {
	return y;
}

float Input::getDX() {
	return dx;
}

float Input::getDY() {
	return dy;
}

float Input::getScroll() {
	return scroll;
}