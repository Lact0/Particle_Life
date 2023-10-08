#include "kernal.h"
#include <cmath>
#include <iostream>

ExpK::ExpK(int w, int h, double a, vector<double> p) {
    width = w;
    height = h;
    alpha = a;
    peaks = p;
    kernal = vector<vector<double>>(width * height, vector<double>(width * height, 0));
    genKernal();
}

vector<double> ExpK::convolve(vector<double> grid) {
    vector<double> convolution;
    return convolution;
}

void ExpK::genKernal() {
    double basis[width * height];
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            double xChange = std::min(i, width - i);
            double yChange = std::min(j, height - j);
            double dist = sqrt(pow(xChange, 2) + pow(yChange, 2));
            double val = peaks[floor(dist * peaks.size())] * shell(fmod(dist * peaks.size(), 1));
            basis[i * width + j] = val;
            mag += val;
        }
    }
    cout << "Basis kernal generated.\n";
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            vector<double>* pointKernal = &kernal[i * width + j];
            for(int k = 0; k < width; k++) {
                for(int l = 0; l < height; l++) {
                    int oldK = (k - i + width) % width;
                    int oldL = (l - j + height) % height;
                    (*pointKernal)[k * width + l] = basis[oldK * width + oldL] / mag;
                }
            }
        }
    }
}

double ExpK::shell(double dist) {
    double den = 4 * dist * (1 - dist);
    return exp(alpha - alpha/den);
}