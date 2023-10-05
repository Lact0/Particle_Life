#include <iostream>
#include <SDL2/SDL.h>
#include "vectorMath.h"
#include "quadTree.h"
#include "particle.h"
#include <cmath>
#include <cstdlib>
#include <thread>

using namespace std;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

const int windowWidth = 500;
const int windowHeight = 250;
const double windowScale = 2;

int colors[6][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 0, 255}, {0, 255, 255}};

double forces[6][6];

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

double tooClose = 3;
double maxDist = 30;
double closeStrength = 20; // 20

Vector getForce(Vector gap, double strength) {
    Vector force;
    if(gap.mag < tooClose) {
        force = gap.norm().mult((gap.mag - tooClose) / tooClose).mult(closeStrength);
    } else if(gap.mag <= maxDist) {
        double halfDist = (maxDist - tooClose) / 2;
        double bestDist = tooClose + halfDist;
        double frac = (halfDist - abs(bestDist - gap.mag)) / halfDist;
        force = gap.norm().mult(frac * strength);
    }
    return force;
}

void workerComputeForces(QuadTree* tree, vector<Particle*>* partPtr, int startInd, int jump) {
    vector<Particle*> particles = *partPtr;
    for(int i = startInd; i < particles.size(); i += jump) {
        Particle* p = particles[i];
        vector<Particle> closeParticles = tree->getClosePoints(*p, maxDist);
        for(Particle np: closeParticles) {
            Vector dir = np.pos.sub(p->pos);
            Vector force = getForce(dir, forces[p->color][np.color]);
            p->applyForce(force);
        }
    }
}

void rollForces() {
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            forces[i][j] = (((double) rand() / (RAND_MAX)) - .5) * 6;
            cout << i << " to " << j << ": " << forces[i][j] << "\n";
        }
    }
}

int main(int argv, char** args) {
    if(!startup()) {
        return 1;
    }

    srand((unsigned) time(NULL));

    SDL_Event e;
    bool running = true;
    int frameStart, frameTime;
    int numColors = 6;
    /*forces = {{3, 0, 0, 0, 0, 0},
                                           {2, 3, 0, 0, 0, 0},
                                           {0, 0, 3, 0, 0, 0},
                                           {0, 0, 2, 3, 0, 0},
                                           {0, 0, 0, 0, 3, 0},
                                           {0, 0, 0, 0, 2, 3}};
    */

    rollForces();
    cout << thread::hardware_concurrency();

    bool showTree = false;

    vector<Particle*> particles;

    for(int i = 0; i < 1000; i++) {
        particles.push_back(new Particle(Vector(rand() % windowWidth, rand() % windowHeight), i % numColors));
    }

    int ticks = 0;
    int newColor = 0;

    while(running) {
        frameStart = SDL_GetTicks();

        if(ticks == 60) {
            ticks = 0;
            particles.push_back(new Particle(Vector(windowWidth / 2, windowHeight / 2), newColor));
            newColor = (newColor + 1) % numColors;
        }

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
                    particles.push_back(new Particle(mousePos, newColor));
                    newColor = (newColor + 1) % numColors;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_SPACE:
                            showTree = !showTree;
                            break;
                        case SDLK_n:
                            particles.clear();
                            for(int i = 0; i < 1000; i++) {
                                particles.push_back(new Particle(Vector(rand() % windowWidth, rand() % windowHeight), i % numColors));
                            }
                            break;
                        case SDLK_r:
                            rollForces();
                            break;
                    }
                    break;
            }
        }

        QuadTree tree(Vector(0, 0), Vector(windowWidth, windowHeight));
        for(Particle* p: particles) {
            tree.addPoint(*p);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for(Particle* p: particles) {
            int col = p->color;
            SDL_SetRenderDrawColor(renderer, colors[col][0], colors[col][1], colors[col][2], 255);
            SDL_RenderDrawPoint(renderer, p->pos.x, p->pos.y);
        }

        if(showTree) {
            tree.drawTree(renderer);
        }

        //START OF PHYSICS UPDATE
        int numThreads = thread::hardware_concurrency();
        thread threads[numThreads];
        for(int i = 0; i < numThreads; i++) {
            threads[i] = thread(workerComputeForces, &tree, &particles, i, numThreads);
        }
        for(int i = 0; i < numThreads; i++) {
            threads[i].join();
        }

        //
        for(Particle* p: particles) {
            double dt = .1;
            if(p->pos.x <= 0 || p->pos.x >= windowWidth - 1) {
                p->vel.x *= -.5;
            }
            if(p->pos.y <= 0 || p->pos.y >= windowHeight - 1) {
                p->vel.y *= -.5;
            }
            p->step(dt);
            p->pos.x = min(max(0, p->pos.x), windowWidth - 1);
            p->pos.y = min(max(0, p->pos.y), windowHeight - 1);
        }

        //END OF PHYSICS UPDATE


        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
        ticks++;
    }

    quit();
    return 0;
}