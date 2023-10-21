#include<iostream>
#include<vector>

using Image = std::vector<std::vector<int>>;

void PrintImage(const Image& image){
    for(const auto& row : image){
        for(const auto& pixel : row){
            std::cout << pixel << " ";
        }
        std::cout << std::endl;
    }
};


