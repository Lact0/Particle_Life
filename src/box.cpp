#include "box.h"

Box::Box(Vector p, Vector d) {
    pos = p;
    diag = d;
}

bool Box::containsPoint(Vector p) {
    Vector opp = pos.add(diag);
    return (p.x >= pos.x) && (p.y >= pos.y) && (p.x < opp.x) && (p.y < opp.y);
}

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

bool Box::overlaps(Box b) {
    return  (max(pos.x, b.pos.x) < min(pos.x + diag.x, b.pos.x + b.diag.x)) &&
            (max(pos.y, b.pos.y) < min(pos.y + diag.y, b.pos.y + b.diag.y));
}

void Box::draw(SDL_Renderer *renderer) {
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + diag.x, pos.y);
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x, pos.y + diag.y);
    SDL_RenderDrawLine(renderer, pos.x + diag.x, pos.y, pos.x + diag.x, pos.y + diag.y);
    SDL_RenderDrawLine(renderer, pos.x, pos.y + diag.y, pos.x + diag.x, pos.y + diag.y);
}