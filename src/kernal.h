#pragma once
#include <vector>

using namespace std;

class Kernal {
    virtual vector<double> convolve(vector<double> grid) = 0;
};

class ExpK : public Kernal {
    public:
        int width;
        int height;
        double alpha;
        vector<double> peaks;

        vector<vector<double>> kernal;

        ExpK(int w, int h, double a, vector<double> p);

        vector<double> convolve(vector<double> grid);
        double shell(double dist);
    
    private:
        double mag;

        void genKernal();
        
};