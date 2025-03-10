#ifndef DECODE_MESSAGE_H
#define DECODE_MESSAGE_H

#include <string>
#include "ImageMatrix.h"

// Class `DecodeMessage`: Provides the functionality to decode a binary message into an ASCII representation.
class DecodeMessage {
    public:
        // Constructors and destructors
        DecodeMessage(); // Default constructor
        ~DecodeMessage(); // Destructor

        static std::string decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels);
    
    private:
        static std::string binaryToASCII(const std::string& binaryString);

};

#endif // DECODE_MESSAGE_H
