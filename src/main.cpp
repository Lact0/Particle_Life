#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>

using namespace std;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

const int windowWidth = 500;
const int windowHeight = 250;
const double windowScale = 2;

bool startup() {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    window = SDL_CreateWindow("Test", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            windowWidth * windowScale, windowHeight * windowScale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, windowScale, windowScale);

    return true;
}

void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argv, char** args) {
    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    double grid[windowWidth][windowHeight];

    while(running) {
        frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:

                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_SPACE:
                            break;
                    }
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Update


        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    quit();
    return 0;
}