//main.cpp
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <map>
#include <iostream>
#include "Main.h"
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
std::vector<SDL_Texture*> Main::tileTexture;
std::vector<std::vector<Tile>> Main::tiles;
void drawTiles(std::vector<std::vector<Tile>>& tileArray, SDL_Renderer* renderer) {
    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            tileArray[x][y].draw(renderer);
        }
    }
}
int main() {
    Main::tiles = std::vector<std::vector<Tile>>(500, std::vector<Tile>(500));
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* window = SDL_CreateWindow("Terroria", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Main::tileTexture.push_back(IMG_LoadTexture(renderer, "dirt.png"));
    int xMax = 0;
    int yMax = 0;
    SDL_Event e;
    int count = 0;
    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            (Main::tiles)[x][y] = Tile(DIRT, x * 1, y * 1);
        }
    }
    int start = SDL_GetTicks();
    int time = 0;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) {

        }
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        drawTiles(Main::tiles, renderer);

        SDL_RenderPresent(renderer);
        count++;
        time = SDL_GetTicks() - start;
        SDL_Delay(1000);
    }
    std::cout << count << "\n";
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    while (1) {}
}
