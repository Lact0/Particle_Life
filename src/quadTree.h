#pragma once
#include "vectorMath.h"
#include "box.h"
#include <vector>

using namespace std;

class QuadTree : public Box {
    public:
        bool isLeaf;
        bool hasPoint;
        Vector point;
        QuadTree* children[4];

        QuadTree(Vector p, Vector d);

        void addPoint(Vector p);
        void drawTree(SDL_Renderer *renderer);
        vector<Vector> getClosePoints(Vector p, double r);
    
    private:
        void split();
        vector<Vector> getClosePoints(Vector p, Box b);

};