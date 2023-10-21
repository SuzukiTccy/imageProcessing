#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include "../Mytool.h"

// two-dimensional image
using Image = std::vector<std::vector<int>>;

// calculate the histogram of image
std::vector<int> calculateHistogram(const Image& image, int L){
    std::vector<int> histogram(L, 0);
    for(const auto& row : image){
        for(const int& pixel_value : row){
            ++histogram[pixel_value];
        }
    }
    return histogram;
}

// histogram equalization
Image histogramEqualization(const Image& image, int L){
    int rows = image.size();
    int cols = image[0].size();
    int N = rows * cols;
    std::vector<int> histogram = calculateHistogram(image, L);

    //calculate CDF
    std::vector<int> cdf(L, 0);
    cdf[0] = histogram[0];
    for(int i = 1; i < L; ++i){
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // normalization CDF
    int cdfmin = *std::min_element(cdf.begin(), cdf.end());
    
    std::vector<float> cdf_norm(L, 0);
    for(int i = 0; i < L; ++i){
        cdf_norm[i] = static_cast<float>(cdf[i] - cdfmin) / (L - cdfmin);
    }

    // calculate new image value
    Image equalized_image(rows, std::vector<int>(cols, 0));
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            int old_pixel = image[i][j];
            int new_pixel = round((L-1) * cdf_norm[old_pixel]);
            equalized_image[i][j] = new_pixel;
        }
    }
    return equalized_image;
}



int main(){
    // example
    Image image = {{0, 34, 125, 1},
                   {1, 43, 125, 2},
                   {2, 53, 250, 3},
                   {3, 63, 250, 4}};
    
    int L = 256;

    // histogram equalization
    Image equalized_image = histogramEqualization(image, L);

    PrintImage(equalized_image);

    return 1;
}
