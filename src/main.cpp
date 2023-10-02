#include <iostream>
#include <SDL2/SDL.h>
#include "vectorMath.h"
#include "quadTree.h"
#include <cmath>

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

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;

    Box b(Vector(0, 0), Vector(50, 50));
    Vector vel(1, 1);

    while(running) {
        frameStart = SDL_GetTicks();

        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector mousePos(x, y);
        mousePos = mousePos.div(windowScale);

        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                running = false;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                cout << mousePos.x << " " << mousePos.y << " ";
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        b.draw(renderer);

        b.pos = b.pos.add(vel);

        if(b.pos.x <= 0 || b.pos.x + b.diag.x >= windowWidth - 1) {
            vel.x *= -1;
        }
        if(b.pos.y <= 0 || b.pos.y + b.diag.y >= windowHeight - 1) {
            vel.y *= -1;
        }

        SDL_RenderPresent(renderer);

        if(b.containsPoint(mousePos)) {
            running = false;
        }

        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    quit();
    return 0;
}