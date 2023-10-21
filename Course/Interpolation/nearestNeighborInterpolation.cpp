#include<iostream>
#include<vector>
#include<cmath>

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
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    int newRows = 6;
    int newCols = 6;
    
    Image newImage = nearestNeighborInterpolation(original, newRows, newCols);

    for(const auto& row : newImage){
        for(const auto& val : row){
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;

}