#include <algorithm>
#include "debug.h"
#define NOMINMAX
#include <iostream>
#include "Vector2.h"
#include "Main.h"
#include "Cursor.h"
#include "Windows.h"

namespace Debug {
    Vector2* positionA = nullptr;
    bool consoleEnabled = true;
    bool getPxCalculationStarted = false;
    bool hiddenWindow = false;
    bool running = true;
    bool vsync = true;
    bool renderHitboxes = true;
    bool lighting = true;
    bool noclip = false;
    bool fullscreen = false;
    float MAX_ENTITY_UPDATE_DIST = 30;
    bool runDebugFunctions = true;
    std::unordered_map<SDL_Keycode, void(*)()> hotKeyToFn = {
        {SDLK_F1,&Debug::outputCursorPos},
        {SDLK_F10,&Debug::toggleHitboxes},
        {SDLK_F9,&Debug::outputWorldDims},
        {SDLK_F8, &Debug::outputScreenDims},
        {SDLK_HOME, &Debug::getPxWidthHeightRelativeToWindow},
        {SDLK_F7,&Debug::getEntityCount},
        {SDLK_F2,&Debug::toggleWindow}
    };
}

void Debug::DebugHotkeyFunctions(SDL_Keycode key) {
    if (Debug::hotKeyToFn.count(key)>0) Debug::hotKeyToFn[key]();
}

void Debug::generateMessageBox(std::wstring title,std::wstring text) {
    MessageBox(NULL,text.c_str(),title.c_str(), MB_OK);
}

void Debug::outputCursorPos() {
    Vector2 scrPos = Cursor::ScreenPos().convertToVector2();
    std::cout << "[DEBUG] CURSOR SCREEN POSITION - ";
    std::cout << "{" << scrPos.X << "," << scrPos.Y << "}";
    std::cout << ", CURSOR WORLD POSITION - ";
    Cursor::WorldPos().out();
}
void Debug::toggleHitboxes() {
    Debug::renderHitboxes = !Debug::renderHitboxes;
    std::cout << "[DEBUG] DISPLAY HITBOXES - ";
    std::cout << ((Debug::renderHitboxes) ? "ENABLED" : "DISABLED") << "\n";
}

void Debug::outputWorldDims() {
    std::cout << "[DEBUG] WORLD WIDTH - " << Main::WORLD_WIDTH << " WORLD HEIGHT - " << Main::WORLD_HEIGHT << "\n";
}

void Debug::outputScreenDims() {
    std::cout << "[DEBUG] WINDOW WIDTH - " << Main::WINDOW_WIDTH << " WINDOW HEIGHT - " << Main::WINDOW_HEIGHT << "\n";
}

void Debug::getEntityCount() {
    std::cout << "[DEBUG] ENTITY COUNT - " << Main::entities.size() << "\n";
}


void Debug::getPxWidthHeightRelativeToWindow() {
    if (!getPxCalculationStarted) {
        Debug::generateMessageBox(L"Do math", L"move cursor to first position and press HOME");
        Debug::getPxCalculationStarted = true;
    }
    else {
        if (Debug::positionA == nullptr) {
            Debug::positionA = new Vector2(Cursor::ScreenPos().convertToVector2());
            Debug::generateMessageBox(L"Do math", L"move cursor to second position and press HOME");
        }
        else {
            Vector2 positionB = Cursor::ScreenPos().convertToVector2();
            double x = std::min(positionA->X, positionB.X);
            double y = std::min(positionA->Y, positionB.Y);
            Vector2 dims = Main::getWidthAndHeight(*Debug::positionA, positionB);
            Vector2 windowDims = { Main::WINDOW_WIDTH,Main::WINDOW_HEIGHT };
            SDL_Rect r = { x,y,dims.X,dims.Y };
            std::wstring res = std::wstring(L"Rectangle values:\nX: ") + std::to_wstring(x) + L"\n Y: " + std::to_wstring(y) + L"\n Width: " + std::to_wstring(dims.X) + L"\n Height: " + std::to_wstring(dims.Y) + L"\n\n";
            x = x / windowDims.X;
            y = y / windowDims.Y;
            dims.X = dims.X / windowDims.X;
            dims.Y = dims.Y / windowDims.Y;
            res += std::wstring(L"Rectangle values:\nX as a fraction of Window Width: ") + std::to_wstring(x) + L"\n Y as a fraction of Window Height: " + std::to_wstring(y) + L"\n Width as a fraction of Window Width: " + std::to_wstring(dims.X) + L"\n Height as a fraction of Window Height: " + std::to_wstring(dims.Y)+L"\n";
            Debug::generateMessageBox(L"Output", res);
            std::wcout << res;
            Debug::getPxCalculationStarted = false;
            delete Debug::positionA;
            Debug::positionA = nullptr;
        }
    }
}

void Debug::toggleWindow() {
    if (Debug::hiddenWindow) {
        SDL_ShowWindow(Main::window);
        std::cout<< "Window Shown\n";
    }
    else {
        SDL_HideWindow(Main::window);
        std::cout << "Window Hidden\n";
    }
    Debug::hiddenWindow = !Debug::hiddenWindow;
 
}


void Debug::enableConsole() {
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}