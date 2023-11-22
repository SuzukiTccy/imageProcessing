#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h"

// Define a two-dimensional grayscale image.
using Image = std::vector<std::vector<int>>;

// Function to apply the Roberts cross operator on an image.
Image applyRobertsOperator(const Image& original) {
    int rows = original.size();
    int cols = original[0].size();

    // Initialize the new image that will store the edge information.
    Image edges(rows, std::vector<int>(cols, 0));

    // Roberts cross gradient operators.
    int gx, gy;

    // Iterate over the image, excluding the border pixels.
    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            // Apply the Roberts cross operator.
            gx = std::abs(original[i][j] - original[i+1][j+1]);
            gy = std::abs(original[i+1][j] - original[i][j+1]);

            // Combine the gradients.
            edges[i][j] = sqrt(gx * gx + gy * gy);
        }
    }

    return edges;
}

int main() {
    // Example usage with a simple 5x5 image.
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

    Image edges = applyRobertsOperator(original);

    // Print the edge-detected image.
    PrintImage(edges, pixel_round);

    return 0;
}
