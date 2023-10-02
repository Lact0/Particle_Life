#pragma once

class Vector {
    public:
        double x;
        double y;
        double mag;

        Vector();
        Vector(double nx, double ny);

        Vector add(Vector v);
        Vector sub(Vector v);
        Vector mult(double n);
        Vector div(double n);
        Vector norm();

    private:
        void genMag();
};