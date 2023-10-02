#include "particle.h"
#include <iostream>

Particle::Particle() {
    pos = Vector();
    color = 0;
}

Particle::Particle(Vector p, int c) {
    pos = p;
    color = c;
}

void Particle::applyForce(Vector f) {
    accel = accel.add(f);
}

void Particle::step(double dt) {
    pos = pos.add(vel.mult(dt)).add(accel.mult(dt * dt / 2));
    vel = vel.add(accel.mult(dt));
    double maxVel = 10;
    if(vel.mag > maxVel) {
        vel = vel.norm().mult(maxVel);
    }
    vel = vel.mult(.99);
    accel = Vector();
}