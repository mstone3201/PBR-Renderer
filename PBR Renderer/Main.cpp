#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include <fstream>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	// open up console and redirect std::cout to it
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	// initialize the game
	Game game(hInstance);
	HRESULT result = game.init();
	if(FAILED(result)) return result;
	return game.loop();
}