#include <iostream>
#include <vector>
#include <cmath>
#include "../Mytool.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846  // the definition of M_PI for windows
#endif

using Image = std::vector<std::vector<int>>;

// Gaussian Filter Function
Image gaussianFilter(const Image& original, int kernelSize, double sigma) {
    int rows = original.size();
    int cols = original[0].size();
    Image filtered(rows, std::vector<int>(cols, 0));

    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double mean = kernelSize / 2.0; //calculate the idx location of kernal
    double sum = 0.0;

    for (int x = 0; x < kernelSize; ++x) {
        for (int y = 0; y < kernelSize; ++y) {
            kernel[x][y] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
                           / (2 * M_PI * sigma * sigma);
            sum += kernel[x][y];
        }
    }

    for (int x = 0; x < kernelSize; ++x)
        for (int y = 0; y < kernelSize; ++y)
            kernel[x][y] /= sum;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value = 0.0;
            for (int u = -mean; u <= mean; ++u) {
                for (int v = -mean; v <= mean; ++v) {
                    int x = std::min(std::max(i + u, 0), rows - 1);
                    int y = std::min(std::max(j + v, 0), cols - 1);
                    value += original[x][y] * kernel[u + mean][v + mean];
                }
            }
            filtered[i][j] = static_cast<int>(value);
        }
    }

    return filtered;
}

int main() {
    // Sample Image
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

    // Parameters for Gaussian Filter
    int kernelSize = 3;
    double sigma = 1.5;

    // Apply Gaussian Filter
    Image filteredImage = gaussianFilter(original, kernelSize, sigma);

    // Print the Filtered Image
    PrintImage(filteredImage, pixel_round);

    return 0;
}
