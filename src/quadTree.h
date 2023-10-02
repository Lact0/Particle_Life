#pragma once
#include "vectorMath.h"
#include "box.h"
#include <vector>

using namespace std;

class QuadTree : public Box {
    public:
        bool isLeaf;
        Vector point;
        QuadTree* children[4];

        QuadTree(Vector p, Vector d);

        void addPoint(Vector point);
        vector<Vector> getClosePoints(Vector point, double r);
    
    private:
        vector<Vector> getClosePoints(Vector point, Box b);

};