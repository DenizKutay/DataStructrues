#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();

    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);


private:
    static std::string decimalToBinary(int &decimal);
    static double binaryToDecimal(std::string& binary);
    static bool isPrime(int i);
};

#endif // ENCODE_MESSAGE_H
