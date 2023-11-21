#include<iostream>
#include<vector>
#include<cmath>
#include "../Mytool.h" // Assuming PrintImage function is defined here

// Define a two-dimensional grayscale image.
using Image = std::vector<std::vector<int>>;

// Helper function to handle image borders
int getPixel(const Image& image, int x, int y) {
    if(x < 0 || y < 0 || x >= image.size() || y >= image[0].size()) {
        return 0; // Assume pixels outside the boundaries are zero
    }
    return image[x][y];
}

// Function to apply the Laplacian operator on an image
Image applyLaplacianOperator(const Image& original) {
    const int rows = original.size();
    const int cols = original[0].size();
    Image edges(rows, std::vector<int>(cols, 0));

    // Laplacian operator kernel
    const std::vector<std::vector<int>> laplacianKernel = {
        { 0,  1,  0},
        { 1, -4,  1},
        { 0,  1,  0}
    };

    for(int x = 1; x < rows - 1; ++x) {
        for(int y = 1; y < cols - 1; ++y) {
            int sum = 0;

            // Apply the Laplacian kernel to the image
            for(int i = -1; i <= 1; ++i) {
                for(int j = -1; j <= 1; ++j) {
                    sum += getPixel(original, x + i, y + j) * laplacianKernel[i + 1][j + 1];
                }
            }

            // Store the result in the edges image
            edges[x][y] = sum;
        }
    }

    return edges;
}

int main() {
    // Example usage with a simple 5x5 image
    Image original = {
        {1, 2, 3, 2, 1},
        {4, 5, 6, 5, 4},
        {7, 8, 9, 8, 7},
        {4, 5, 6, 5, 4},
        {1, 2, 3, 2, 1}
    };

    Image edges = applyLaplacianOperator(original);

    // Print the edge-detected image
    int pixel_round = 10; // Example maximum pixel value for normalization
    PrintImage(edges, pixel_round);

    return 0;
}
