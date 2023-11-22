#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// two-dimensional grayscale image
using Image = std::vector<std::vector<int>>;

// Helper function to handle image borders
int getPixel(const Image& image, int x, int y) {
    if(x < 0 || y < 0 || x >= image.size() || y >= image[0].size()) {
        return 0; // 边界外的像素值假设为0
    }
    return image[x][y];
}

// Function to apply the Prewitt operator on an image
Image applyPrewittOperator(const Image& original) {
    const int size = original.size();
    const int colSize = original[0].size();
    Image edges(size, std::vector<int>(colSize, 0));

    // Prewitt operator kernels for x and y directions
    const std::vector<std::vector<int>> prewittY = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    const std::vector<std::vector<int>> prewittX = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

    for(int x = 1; x < size - 1; ++x) {
        for(int y = 1; y < colSize - 1; ++y) {
            int gx = 0;
            int gy = 0;

            // Apply each Prewitt kernel to the image
            for(int i = -1; i <= 1; ++i) {
                for(int j = -1; j <= 1; ++j) {
                    int pixel = getPixel(original, x + i, y + j);
                    gx += pixel * prewittX[i + 1][j + 1];
                    gy += pixel * prewittY[i + 1][j + 1];
                }
            }

            // Calculate the gradient magnitude
            edges[x][y] = std::sqrt(gx * gx + gy * gy);
        }
    }

    return edges;
}

int main() {
    // Example usage with a simple 5x5 image
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


    int pixel_round = 255;
    
    Image edges = applyPrewittOperator(original);

    // Print the edge-detected image
    PrintImage(edges, pixel_round);

    return 0;
}
