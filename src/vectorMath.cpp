#include "vectorMath.h"
#include <cmath>

/*
Possible Improvements:

Don't generate mag when mult, div, norm

*/

Vector::Vector() {
    x = 0;
    y = 0;
    mag = 0;
}
Vector::Vector(double nx, double ny) {
    x = nx;
    y = ny;
    genMag();
}

Vector Vector::add(Vector v) {
    return Vector(x + v.x, y + v.y);
}
Vector Vector::sub(Vector v) {
    return Vector(x - v.x, y - v.y);
}
Vector Vector::mult(double n) {
    return Vector(x * n, y * n);
}
Vector Vector::div(double n) {
    return Vector(x / n, y / n);
}
Vector Vector::norm() {
    if(mag == 0) {
        return Vector();
    }
    return Vector(x / mag, y / mag);
}

void Vector::genMag() {
    mag = sqrt(pow(x, 2) + pow(y, 2));
}