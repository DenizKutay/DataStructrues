// EdgeDetector.cpp
#include "EdgeDetector.h"
#include <cmath>
#include <iostream>
#include "ImageSharpening.h"

// Default constructor
EdgeDetector::EdgeDetector() {

}

// Destructor
EdgeDetector::~EdgeDetector() {
    if (xKernel_data != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] xKernel_data[i];
        }
    }
    if (yKernel_data != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] yKernel_data[i];
        }
    }

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    Convolution xCon(xKernel_data,3,3,1,true);
    ImageMatrix Ix = xCon.convolve(input_image);

    Convolution yCon(yKernel_data,3,3,1,true);
    ImageMatrix Iy = yCon.convolve(input_image);

    ImageMatrix magnitudeMatrix(input_image.get_height(), input_image.get_width());
    double sum = 0;
    for (int i = 0; i < magnitudeMatrix.get_height(); i++) {
        for (int j = 0; j < magnitudeMatrix.get_width(); j++) {
            double magnitude;
            magnitude = std::sqrt((Ix.get_data(i,j) * Ix.get_data(i,j)) + (Iy.get_data(i,j) * Iy.get_data(i,j)));
            magnitudeMatrix.set_data(i, j, magnitude);
            sum += magnitude;
        }
    }
    std::vector<std::pair<int,int>> result;
    thresholdValue = sum / (magnitudeMatrix.get_height() * magnitudeMatrix.get_width());
    for(int i = 0; i < magnitudeMatrix.get_height(); i++) {
        for(int j = 0; j < magnitudeMatrix.get_width(); j++) {
            if (magnitudeMatrix.get_data(i, j) > thresholdValue) {
                result.push_back(std::make_pair(i,j));
            }
        }
    }
    return result;
}


