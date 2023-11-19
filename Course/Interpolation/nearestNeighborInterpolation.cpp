#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// two-dimensional image
using Image = std::vector<std::vector<int>>;

Image nearestNeighborInterpolation(const Image& original, int newRows, int newCols) {
    int oldrows = original.size();
    int oldcols = original[0].size();

    float rowsScale = static_cast<float>(oldrows) / newRows;
    float colsScale = static_cast<float>(oldcols) / newCols;

    Image newImage(newRows, std::vector<int>(newCols, 0));
    for(int i = 0; i < newRows; i++) {
        for(int j = 0; j < newCols; j++){
            int old_i = std::round(i * rowsScale);
            int old_j = std::round(j * colsScale);

            old_i = std::min(old_i, oldrows - 1);
            old_j = std::min(old_j, oldcols - 1);

            newImage[i][j] = original[old_i][old_j];

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
        {6, 7, 8, 9, 1}};

    int newRows = 10;
    int newCols = 10;
    
    Image newImage = nearestNeighborInterpolation(original, newRows, newCols);

    int pixel_round = 10;

    PrintImage(newImage, pixel_round);

    return 0;

}