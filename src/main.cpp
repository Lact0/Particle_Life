#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "growth.h"
#include "kernal.h"
#include <cmath>

using namespace std;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

const int windowWidth = 200;
const int windowHeight = 200;
const double windowScale = 4;

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

    ExpK k(windowWidth, windowHeight, 4, vector<double>({1}));

    cout << "Finished making the kernal!!";

    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    vector<vector<double>> grid;

    ExpGF g(.25, .03);
    
    while(running) {
        frameStart = SDL_GetTicks();

        int x, y;
        SDL_GetMouseState(&x, &y);
        x /= windowScale;
        y /= windowScale;

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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int i = 0; i < windowWidth; i++) {
            for(int j = 0; j < windowHeight; j++) {
                double col = 255 * k.kernal[x * windowWidth + y][i * windowHeight + j] * 1000;
                SDL_SetRenderDrawColor(renderer, col, col, col, 255);
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }

        SDL_RenderPresent(renderer);
        
        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    quit();
    return 0;
}