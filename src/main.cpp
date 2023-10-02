#include <iostream>
#include <SDL2/SDL.h>
#include "vectorMath.h"
#include "quadTree.h"
#include <cmath>
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

    QuadTree tree(Vector(0, 0), Vector(windowWidth, windowHeight));
    vector<Vector> points;

    for(int i = 0; i < 10000; i++) {
        Vector newPoint(rand() % windowWidth, rand() % windowHeight);
        points.push_back(newPoint);
        tree.addPoint(newPoint);
    }

    bool showTree = false;

    while(running) {
        frameStart = SDL_GetTicks();

        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector mousePos(x, y);
        mousePos = mousePos.div(windowScale);

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    tree.addPoint(mousePos);
                    points.push_back(mousePos);
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_SPACE:
                            showTree = !showTree;
                            break;
                    }
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(Vector point: points) {
            SDL_RenderDrawPoint(renderer, point.x, point.y);
        }
        if(showTree) {
            tree.drawTree(renderer);
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        vector<Vector> closePoints = tree.getClosePoints(mousePos, 25);
        for(Vector point: closePoints) {
            SDL_RenderDrawPoint(renderer, point.x, point.y);
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