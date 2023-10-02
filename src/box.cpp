#include "box.h"

Box::Box(Vector p, Vector d) {
    pos = p;
    diag = d;
}

bool Box::containsPoint(Vector p) {
    Vector opp = pos.add(diag);
    return (p.x >= pos.x) && (p.y >= pos.y) && (p.x < opp.x) && (p.y < opp.y);
}

void Box::draw(SDL_Renderer *renderer) {
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + diag.x, pos.y);
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x, pos.y + diag.y);
    SDL_RenderDrawLine(renderer, pos.x + diag.x, pos.y, pos.x + diag.x, pos.y + diag.y);
    SDL_RenderDrawLine(renderer, pos.x, pos.y + diag.y, pos.x + diag.x, pos.y + diag.y);
}