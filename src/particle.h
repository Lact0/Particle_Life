#pragma once
#include "vectorMath.h"

class Particle {
    public:
        Vector pos;
        Vector vel;
        Vector accel;
        int color;

        Particle();
        Particle(Vector p, int c);

        void applyForce(Vector f);
        void step(double dt);

};