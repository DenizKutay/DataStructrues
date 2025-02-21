// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <cmath>
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binary;
    for(std::pair<int,int> pair :edgePixels) {
        int lsb = ((int)image.get_data(pair.first, pair.second) % 2 );
        binary += std::to_string(lsb);
    }
    while (binary.size() % 7 != 0) {
        binary = "0" + binary;
    }
    std::string result = binaryToASCII(binary);
    return result;
}

    

std::string DecodeMessage::binaryToASCII(const std::string &binaryString) {
    std::string result;
    std::string kutay = binaryString;
    while (kutay.size() >= 7) {
        int decimal = 0;
        int exponent = 0;
        for (int i = 6; i >= 0; i--) {
            if (kutay[i] == '1') {
                decimal += static_cast<int>(pow(2, exponent));
            }
            exponent++;
        }
        kutay.erase(0,7);
        if (decimal <= 32) {
            decimal += 33;
        } else if (decimal >= 127) {
            decimal = 126;
        }
        char character = static_cast<char>(decimal);
        std::string charToString(1,character);
        result += charToString;
    }
    return result;
}

