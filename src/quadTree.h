#pragma once
#include "vectorMath.h"
#include "box.h"
#include "particle.h"
#include <vector>

using namespace std;

class QuadTree : public Box {
    public:
        bool isLeaf;
        bool hasPoint;
        Particle point;
        QuadTree* children[4];

        QuadTree(Vector p, Vector d);

        void addPoint(Particle p);
        void drawTree(SDL_Renderer *renderer);
        vector<Particle> getClosePoints(Particle p, double r);
    
    private:
        void split();
        vector<Particle> getClosePoints(Particle p, Box b);

};