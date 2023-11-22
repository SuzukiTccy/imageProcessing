#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// two-dimensional image
using Image = std::vector<std::vector<int>>;

Image bilinearInterpolation(const Image& original, int newRows, int newCols) {
    int oldRows = original.size();
    int oldCols = original[0].size();

    float rowScale = static_cast<float>(oldRows) / newRows;
    float colScale = static_cast<float>(oldCols) / newCols;

    Image newImage(newRows, std::vector<int>(newCols, 0));

    for(int i = 0; i < newRows; ++i) {
        for(int j = 0; j < newCols; ++j) {
            float old_i = i * rowScale;
            float old_j = j * colScale;

            int i0 = static_cast<int>(floor(old_i));
            int i1 = static_cast<int>(ceil(old_i));
            int j0 = static_cast<int>(floor(old_j));
            int j1 = static_cast<int>(ceil(old_j));

            // Boundary conditions
            i1 = std::min(i1, oldRows - 1);
            j1 = std::min(j1, oldCols - 1);

            // Interpolation weights
            float xWeight = old_i - i0;
            float yWeight = old_j - j0;

            // Interpolate along x for both sets
            float top = (1 - xWeight) * original[i0][j0] + xWeight * original[i0][j1];
            float bottom = (1 - xWeight) * original[i1][j0] + xWeight * original[i1][j1];

            // Interpolate along y
            newImage[i][j] = std::round((1 - yWeight) * top + yWeight * bottom);
        }
    }

    return newImage;
}

int main(){
    Image original = {
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {255, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 255},
        {255, 100, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  100, 255},
        {255, 100, 50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   50,  100, 255},
        {255, 100, 50,  0,   100, 100, 100, 100, 100, 100, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 255, 255, 255, 255, 255, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 255, 100, 100, 100, 255, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 255, 100, 50,  100, 255, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 255, 100, 100, 100, 255, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 255, 255, 255, 255, 255, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   100, 100, 100, 100, 100, 100, 100, 0,   50,  100, 255},
        {255, 100, 50,  0,   0,   0,   0,   0,   0,   0,   0,   0,   50,  100, 255},
        {255, 100, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  100, 255},
        {255, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 255},
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
    };

    int newRows = 10;
    int newCols = 10;
    
    Image newImage = bilinearInterpolation(original, newRows, newCols);

    int pixel_round = 255;

    PrintImage(newImage, pixel_round);

    return 0;
}