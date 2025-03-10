// Convolution.h

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "ImageMatrix.h"

// Class `Convolution`: Provides the functionality to convolve an image with
// a kernel. Padding is a bool variable, indicating whether to use zero padding or not.
class Convolution {
public:
    // Constructors and destructors
    Convolution(); // Default constructor
    Convolution(double** customKernel, int kernelHeight, int kernelWidth, int stride, bool padding); // Parametrized constructor for custom kernel and other parameters
    ~Convolution(); // Destructor

    Convolution(const Convolution &other); // Copy constructor
    Convolution& operator=(const Convolution &other); // Copy assignment operator

    ImageMatrix convolve(const ImageMatrix& input_image); // Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
    ImageMatrix padding(const ImageMatrix& inputMatrix);

private:

    bool pad;
    int stride_val;
    double** customKernel;
    int kh;
    int kw;
    // Add any private member variables and functions .
};

#endif // CONVOLUTION_H
