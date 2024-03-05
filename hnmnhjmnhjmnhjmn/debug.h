#pragma once
#include <SDL_keycode.h>
#include <unordered_map>
#include <string>



namespace Debug {
	extern bool vsync;
	extern bool renderHitboxes;
	extern bool noclip;
	extern bool lighting;
	extern bool fullscreen;
	extern bool running;
	extern bool consoleEnabled;
	extern float MAX_ENTITY_UPDATE_DIST;
	extern bool runDebugFunctions;
	void DebugHotkeyFunctions(SDL_Keycode key);
	void outputCursorPos();
	void toggleHitboxes();
	void outputScreenDims();
	void getEntityCount();
	void outputWorldDims();
	void getPxWidthHeightRelativeToWindow();
	void toggleWindow();
	void generateMessageBox(std::wstring title,std::wstring text);
	void enableConsole();
	extern std::unordered_map<SDL_Keycode, void(*)()> hotKeyToFn;
}