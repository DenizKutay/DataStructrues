#include <iostream>

#include "Convolution.h"

// Default constructor
Convolution::Convolution() {
    pad = false;
    stride_val = 0;
    kh = 0;
    kw = 0;
    customKernel = nullptr;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    this->pad = pad;
    this->stride_val = stride_val;
    this->kh = kh;
    this->kw = kw;
    this-> customKernel = new double * [kh];
    for(int i = 0; i < kh;i++) {
        this->customKernel[i] = new double [kw];
        for(int j = 0; j < kw;j++) {
            this->customKernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (customKernel != nullptr) {
        for (int i = 0; i < kh; ++i) {
            delete[] customKernel[i];
        }
        delete[] customKernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other) {
    pad = other.pad;
    stride_val = other.stride_val;
    kh = other.kh;
    kw = other.kw;
    customKernel = new double *[kh];
    for (int i = 0; i < kh; i++) {
        customKernel[i] = new double[kw];
        for (int j = 0; j < kw; j++) {
            customKernel[i][j] = other.customKernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    pad = other.pad;
    stride_val = other.stride_val;
    kh = other.kh;
    kw = other.kw;
    customKernel = new double * [kh];
    for(int i = 0; i < kh;i++) {
        customKernel[i] = new double [kw];
        for(int j = 0; j < kw;j++) {
            customKernel[i][j] = other.customKernel[i][j];
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) {
    ImageMatrix copy = padding(input_image);
    int height = ((copy.get_height() - kh) / stride_val) + 1;
    int width = ((copy.get_width() - kw) / stride_val) + 1;
    ImageMatrix result(height, width);
    for (int i = 0;i < result.get_height();i++) {
        for(int j = 0;j < result.get_width();j++) {
            //kernel
            double sum = 0;
            for (int k = 0;k < kh;k++) {
                for (int l = 0;l < kw; l++) {
                    sum += customKernel[k][l] * copy.get_data(((i * stride_val) + k) ,((j * stride_val) + l));
                }
            }
            result.set_data(i,j,sum);
        }
    }
    return result;
}

ImageMatrix Convolution::padding(const ImageMatrix& inputMatrix) {
    if (!pad) {
        return inputMatrix;
    } else {
        ImageMatrix result(inputMatrix.get_height() + 2, inputMatrix.get_width() + 2);
        for (int i = 0; i < result.get_height(); i++) {
            for (int j = 0; j < result.get_width(); j++) {
                if ((i == 0 || i == result.get_height() - 1) || (j == 0 || j == result.get_width() - 1)) {
                    result.set_data(i, j, 0);
                } else {
                    result.set_data(i, j, inputMatrix.get_data()[i - 1][j - 1]);
                }
            }
        }
        return result;
    }
}

