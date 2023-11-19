#include<iostream>
#include<vector>
#include<iomanip>

using Image = std::vector<std::vector<int>>;

void PrintImage(const Image& image, int pixel_round){
    int n = 0;
    while(pixel_round){
        pixel_round /= 10;
        ++n;
    }

    for(const auto& row : image){
        for(const auto& pixel : row){
            std::cout << std::setfill('0') << std::setw(n) << pixel << " ";
        }
        std::cout << std::endl;
    }
};


