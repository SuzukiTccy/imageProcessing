#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include "../Mytool.h"

using Image = std::vector<std::vector<int>>;

// Helper function to get pixel value with boundary checks
int getPixel(const Image& image, int x, int y) {
    if(x < 0 || x >= image.size() || y < 0 || y >= image[0].size()) {
        return 0;
    }
    return image[x][y];
}

// Function to apply Gaussian filter
Image applyGaussianFilter(const Image& image) {
    int rows = image.size();
    int cols = image[0].size();
    Image result = image; // Create a copy to store results

    // Define a 3x3 Gaussian kernel with sigma = 1 (normalized)
    std::vector<std::vector<float>> kernel = {
        {1.0 / 16, 2.0 / 16, 1.0 / 16},
        {2.0 / 16, 4.0 / 16, 2.0 / 16},
        {1.0 / 16, 2.0 / 16, 1.0 / 16}
    };

    for(int i = 1; i < rows - 1; ++i){
        for(int j = 1; j < cols - 1; ++j){
            float sum = 0.0;
            
            // Apply the Gaussian kernel
            for(int ki = -1; ki < 2; ++ki){
                for(int kj = -1; kj < 2; ++kj){
                    sum += getPixel(image, i + ki, j + kj) * kernel[ki + 1][kj + 1];
                }
            }
        
        result[i][j] = static_cast<int>(sum);
        }
    }
    return result;
}

// Function to compute gradients and their directions
void computeGradients(const Image& image, Image& gradientMagnitude, Image& gradientDirection) {
    // Implement gradient computation
    const int rows = image.size();
    const int cols = image[0].size();
    
    // Resize output images to match the input image size
    gradientMagnitude.resize(rows, std::vector<int>(cols, 0));
    gradientDirection.resize(rows, std::vector<int>(cols, 0));

    // Sobel operator kernels
    const std::vector<std::vector<int>> sobelX = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    const std::vector<std::vector<int>> sobelY = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for(int x = 1; x < rows - 1; ++x) {
        for(int y = 1; y < cols - 1; ++y) {
            int gx = 0;
            int gy = 0;

            // Apply Sobel kernels
            for(int i = -1; i <= 1; ++i) {
                for(int j = -1; j <= 1; ++j) {
                    int pixel = getPixel(image, x + i, y + j);
                    gx += pixel * sobelX[i + 1][j + 1];
                    gy += pixel * sobelY[i + 1][j + 1];
                }
            }

            // Calculate gradient magnitude and direction
            gradientMagnitude[x][y] = std::sqrt(gx * gx + gy * gy);
            gradientDirection[x][y] = std::atan2(gy, gx); // radians
        }
    }
}

// Helper function for bilinear interpolation
float bilinearInterpolate(const Image& image, float x, float y) {
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    float dx = x - x0;
    float dy = y - y0;

    float value = (1 - dx) * (1 - dy) * image[x0][y0] +
                  dx * (1 - dy) * image[x0][y1] +
                  (1 - dx) * dy * image[x1][y0] +
                  dx * dy * image[x1][y1];

    return value;
}

// Function for non-maximum suppression
Image nonMaximumSuppression(const Image& gradientMagnitude, const Image& gradientDirection, const std::string& mode="nearest") {
    // Implement non-maximum suppression
    const int rows = gradientMagnitude.size();
    const int cols = gradientMagnitude[0].size();

    Image result(rows, std::vector<int>(cols, 0));

    if(mode == "nearest"){
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
            int q = 255, r = 255;
            float angle = gradientDirection[i][j]; // radians

            // Find the two adjacent pixels to inspect
            if ((angle >= -M_PI / 8 && angle <= M_PI / 8) || (angle >= 7 * M_PI / 8 || angle <= -7 * M_PI / 8)) {
                // Horizontal edge
                q = gradientMagnitude[i][j + 1];
                r = gradientMagnitude[i][j - 1];
            } else if ((angle > M_PI / 8 && angle < 3 * M_PI / 8) || (angle > -7 * M_PI / 8 && angle < -5 * M_PI / 8)) {
                // Diagonal (top right to bottom left)
                q = gradientMagnitude[i + 1][j - 1];
                r = gradientMagnitude[i - 1][j + 1];
            } else if ((angle >= 3 * M_PI / 8 && angle <= 5 * M_PI / 8) || (angle >= -5 * M_PI / 8 && angle <= -3 * M_PI / 8)) {
                // Vertical edge
                q = gradientMagnitude[i + 1][j];
                r = gradientMagnitude[i - 1][j];
            } else if ((angle > 5 * M_PI / 8 && angle < 7 * M_PI / 8) || (angle > -3 * M_PI / 8 && angle < -M_PI / 8)) {
                // Diagonal (top left to bottom right)
                q = gradientMagnitude[i - 1][j - 1];
                r = gradientMagnitude[i + 1][j + 1];
            }


            // Suppress non-maximum values
            if (gradientMagnitude[i][j] >= q && gradientMagnitude[i][j] >= r) {
                result[i][j] = gradientMagnitude[i][j];
            } else {
                result[i][j] = 0;
            }
            }
        }
    }
    else{
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                float angle = gradientDirection[i][j];
                float q = 255, r = 255;

                // Calculate coordinates of pixels in gradient direction
                float x0 = i + cos(angle);  //Polar to Cartesian conversion
                float y0 = j + sin(angle);
                float x1 = i - cos(angle);
                float y1 = j - sin(angle);

                // Perform bilinear interpolation
                if (x0 >= 0 && x0 < rows && y0 >= 0 && y0 < cols) {
                    q = bilinearInterpolate(gradientMagnitude, x0, y0);
                }
                if (x1 >= 0 && x1 < rows && y1 >= 0 && y1 < cols) {
                    r = bilinearInterpolate(gradientMagnitude, x1, y1);
                }

                // Suppress non-maximum values
                if (gradientMagnitude[i][j] >= q && gradientMagnitude[i][j] >= r) {
                    result[i][j] = gradientMagnitude[i][j];
                } else {
                    result[i][j] = 0;
                }
            }
        }
    }
    return result;
}

// Function for double thresholding
Image doubleThresholding(const Image& image, int lowThreshold, int highThreshold) {
    // Implement double thresholding
    const int rows = image.size();
    const int cols = image[0].size();
    Image result(rows, std::vector<int>(cols, 0));

    // Iterate through every pixel in the image
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // If the pixel value is above the high threshold, it is marked as a strong edge
            if (image[i][j] > highThreshold) {
                result[i][j] = 255;
            }
            // If the pixel value is between the low and high thresholds, it is marked as a weak edge
            else if (image[i][j] > lowThreshold) {
                result[i][j] = 100; // Different values can be used to represent weak edges
            }                       // it relate to real application
            // In other cases, suppress the pixel (keep it at 0)
        }
    }
    return result;
}

void applyHysteresisImpl(Image& image, int x, int y) {
    const int rows = image.size();
    const int cols = image[0].size();

    // Neighbors in the 8 direction
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;

        // Check the boundary conditions
        if (newX >= 0 && newX < rows && newY >= 0 && newY < cols) {
            if (image[newX][newY] == 100) { // weak edge
                image[newX][newY] = 255; // strong edge
                applyHysteresisImpl(image, newX, newY); // Recursive marking
            }
        }
    }
}

// Function to apply hysteresis
Image applyHysteresis(const Image& image) {
    // Implement hysteresis
    Image result = image;
    const int rows = image.size();
    const int cols = image[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (result[i][j] == 255) { // string edge
                applyHysteresisImpl(result, i, j); // Recursively marks the weak edges of the connection
            }
        }
    }

    // Set all pixels that are not marked as strong edges to 0
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (result[i][j] != 255) {
                result[i][j] = 0;
            }
        }
    }
    return result;
}

// Main Canny edge detection function
Image cannyEdgeDetection(const Image& original, int lowThreshold, int highThreshold) {
    Image smoothed = applyGaussianFilter(original);
    Image gradientMagnitude, gradientDirection;

    computeGradients(smoothed, gradientMagnitude, gradientDirection);

    Image nonMaxSuppressed = nonMaximumSuppression(gradientMagnitude, gradientDirection);
    Image thresholded = doubleThresholding(nonMaxSuppressed, lowThreshold, highThreshold);
    Image edges = applyHysteresis(thresholded);

    return edges;
}

int main() {
    // Example usage with a simple image
    Image original = {
        {1, 2, 3, 6, 7},
        {4, 5, 6, 1, 2},
        {7, 8, 9, 3, 2},
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 1}
    };

    int pixel_round = 10;

    int lowThreshold = 50;   // Example low threshold value
    int highThreshold = 100; // Example high threshold value

    Image edges = cannyEdgeDetection(original, lowThreshold, highThreshold);

    // Print the edge-detected image
    PrintImage(edges, pixel_round);

    return 0;
}