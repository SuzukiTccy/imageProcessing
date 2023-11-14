#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// two-dimensional image
using Image = std::vector<std::vector<int>>;

Image bilinearInterpolation(const Image& original, int newRows, int newCols) {
    int oldRows = original.size();
    int oldCols = original[0].size();

    float rowScale = static_cast<float>(oldRows - 1) / (newRows - 1);
    float colScale = static_cast<float>(oldCols - 1) / (newCols - 1);

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
        {1, 2, 3, 6, 7},
        {4, 5, 6, 1, 2},
        {7, 8, 9, 3, 2},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 1}
    };

    int newRows = 10;
    int newCols = 10;
    
    Image newImage = bilinearInterpolation(original, newRows, newCols);

    PrintImage(newImage);

    return 0;
}