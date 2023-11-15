#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// two-dimensional image
using Image = std::vector<std::vector<int>>;

// Helper function to perform cubic interpolation
double cubicInterpolate(double v0, double v1, double v2, double v3, double fraction) {
    double A = (v3 - v2) - (v0 - v1);
    double B = (v0 - v1) - A;
    double C = v2 - v0;
    double D = v1;

    return A * pow(fraction, 3) + B * pow(fraction, 2) + C * fraction + D;
}


double cubicInterpolateFunc(const double& fraction, const double& a = -0.5){
    
    double x = std::abs(fraction);
    if(x <= 1){
        return (a+2)*pow(x, 3) - (a+3)*pow(x, 2) + 1;
    }
    else if(x > 1 && x < 2){
        return a*pow(x, 3) - 5*a*pow(x, 2) + 8*a*x - 4*a;
    }
    
    return 0;
}


// Function to perform bicubic interpolation
Image bicubicInterpolation(const Image& original, int newRows, int newCols) {
    int oldRows = original.size();
    int oldCols = original[0].size();

    float rowScale = static_cast<float>(oldRows) / newRows;
    float colScale = static_cast<float>(oldCols) / newCols;

    Image newImage(newRows, std::vector<int>(newCols, 0));

    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            float old_i = i * rowScale;
            float old_j = j * colScale;

            int i1 = static_cast<int>(floor(old_i));
            int j1 = static_cast<int>(floor(old_j));
            int i0 = i1 - 1, i2 = i1 + 1, i3 = i1 + 2;
            int j0 = j1 - 1, j2 = j1 + 1, j3 = j1 + 2;

            // Enforce boundary conditions
            i0 = std::max(i0, 0); i1 = std::max(i1, 0); i2 = std::max(i2, 0); i3 = std::max(i3, 0);
            i0 = std::min(i0, oldRows - 1); i1 = std::min(i1, oldRows - 1); i2 = std::min(i2, oldRows - 1); i3 = std::min(i3, oldRows - 1);
            j0 = std::max(j0, 0); j1 = std::max(j1, 0); j2 = std::max(j2, 0); j3 = std::max(j3, 0);
            j0 = std::min(j0, oldCols - 1); j1 = std::min(j1, oldCols - 1); j2 = std::min(j2, oldCols - 1); j3 = std::min(j3, oldCols - 1);

            // 16*16 pixel in oldimage for interpolation
            int pi[4] = {i0, i1, i2, i3};
            int pj[4] = {j0, j1, j2, j3};
            
            double value = 0.0;
            for(int i = 0; i < 4; ++i){
                for(int j = 0; j < 4; ++j){
                    value += original[pi[i]][pj[j]] * cubicInterpolateFunc(old_i - pi[i]) * cubicInterpolateFunc(old_j - pj[j]);
                }
            }

            
            // Assign the value to new image
            value = std::max(0.0, std::min(value, 255.0));
            newImage[i][j] = static_cast<int>(value);
        }
    }

    return newImage;
}

int main() {
    Image original = {
        {1, 2, 3, 6, 7},
        {4, 5, 6, 1, 2},
        {7, 8, 9, 3, 2},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 1}
    };

    int newRows = 10;
    int newCols = 10;
    
    Image newImage = bicubicInterpolation(original, newRows, newCols);

    PrintImage(newImage);

    return 0;
}