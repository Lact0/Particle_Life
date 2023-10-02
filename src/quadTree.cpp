#include "quadTree.h"

QuadTree::QuadTree(Vector p, Vector d) : Box(p, d) {
    isLeaf = true;
    hasPoint = false;
}

void QuadTree::addPoint(Vector p) {
    if(!containsPoint(p)) {
        return;
    }

    if(isLeaf && hasPoint) {
        if(point.x == p.x && point.y == p.y) {
            return;
        }
        isLeaf = false;
        split();
        for(QuadTree* tree : children) {
            tree->addPoint(p);
        }
    } else if(isLeaf && !hasPoint) {
        point = p;
        hasPoint = true;
    } else {
        for(QuadTree* tree : children) {
            tree->addPoint(p);
        }
    }
}

void QuadTree::drawTree(SDL_Renderer *renderer) {
    draw(renderer);
    if(!isLeaf) {
        for(QuadTree* child: children) {
            child->drawTree(renderer);
        }
    }
}

vector<Vector> QuadTree::getClosePoints(Vector p, double r) {
    Box b(Vector(p.x - r, p.y - r), Vector(r * 2, r * 2));
    return getClosePoints(p, b);
}

void QuadTree::split() {
    isLeaf = false;
    Vector newDiag = diag.div(2);
    children[0] = new QuadTree(pos, newDiag);
    children[1] = new QuadTree(pos.add(Vector(newDiag.x, 0)), newDiag);
    children[2] = new QuadTree(pos.add(Vector(0, newDiag.y)), newDiag);
    children[3] = new QuadTree(pos.add(Vector(newDiag.x, newDiag.y)), newDiag);
    if(hasPoint) {
        addPoint(point);
    }
}

vector<Vector> QuadTree::getClosePoints(Vector p, Box b) {
    vector<Vector> points;
    if(isLeaf && hasPoint && b.containsPoint(point)) {
        points.push_back(point);
    } if(!isLeaf) {
        for(QuadTree* child: children) {
            if(!b.overlaps(*child)) {
                continue;
            }
            vector<Vector> childPoints = child->getClosePoints(p, b);
            points.insert( points.end(), childPoints.begin(), childPoints.end());
        }
    }
    return points;
}
