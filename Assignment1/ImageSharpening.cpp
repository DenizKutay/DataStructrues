#include <iostream>
#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
}

ImageSharpening::~ImageSharpening(){
    for (int i = 0; i < 3; i++) {
        delete[] kernel_data[i];
        kernel_data[i] = nullptr;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    ImageMatrix kernel(kernel_data,3,3);
    kernel  = kernel * (1.0/9.0);
    Convolution blurredCon(kernel.get_data(), 3, 3, 1, true);
    ImageMatrix blurredImg = blurredCon.convolve(input_image);
    ImageMatrix sa = input_image - blurredImg;
    sa = sa * k;
    ImageMatrix sharpImg(input_image.get_height(),input_image.get_width());
    sharpImg = input_image + sa;
    sharpImg.preventOverflowing();
    return sharpImg;
}
