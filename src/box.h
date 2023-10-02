#pragma once
#include "vectorMath.h"
#include <SDL2/SDL.h>

class Box {
    public:
        Vector pos;
        Vector diag;

        Box(Vector p, Vector d);

        bool containsPoint(Vector p);
        void draw(SDL_Renderer *renderer);
};