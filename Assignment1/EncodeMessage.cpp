#include "EncodeMessage.h"
#include <cmath>



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    //Fibonacci number
    int first = 0;
    int second = 1;
    std::string result;
    //character transformation
    for (int i = 0 ; i < message.length();i++) {
        int fib;
        if (i <= 1) {
            fib = i;
        } else {
            fib = first + second;
            first = second;
            second = fib;
        }
            //translate char to int
            char character = message[i];
            int ascii = static_cast<int>(character);

            //check if it is prime
            if (isPrime(i)) {
                ascii += fib;
            }

            //for preventing overflow
            if (ascii <= 32) {
                ascii += 33;
            } else if (ascii >= 127) {
                ascii = 126;
            }

            //new char
            char new_character = static_cast<char>(ascii);
            std::string charToString(1, new_character);
            result += charToString;
    }

    //right circular shifting
    int splitIndex = result.length() / 2;
    std::string shiftedMessage = result.substr(result.length() - splitIndex) + result.substr(0, result.length() - splitIndex);
    result = shiftedMessage;
    std::string binaryMessage;
    for (int i = 0;i < result.length(); i++) {
        char sa = result[i];
        int ascii = int(sa);
        std::string binary = decimalToBinary(ascii);
        while(binary.length() < 7) {
            binary = "0" + binary;
        }
        binaryMessage = binaryMessage + binary;
    }
    //binary
    int edgeCount = 0;
    ImageMatrix hiddenMatrix(img);
    int posCount = 0;
    for(std::pair<int,int> pair :positions) {
        if (posCount == binaryMessage.length()) {
            break;
        }
        int value = img.get_data(pair.first,pair.second);
        std::string binary = decimalToBinary(value);
        binary = binary.substr(0,binary.length() - 1);
        binary += binaryMessage[edgeCount];
        edgeCount++;
        double hiddenValue = binaryToDecimal(binary);
        hiddenMatrix.set_data(pair.first,pair.second,hiddenValue);
        posCount++;
    }
    return hiddenMatrix;
}

std::string EncodeMessage::decimalToBinary(int &decimal) {
    std::string result;
    if(decimal == 0) {
        return "0";
    }
    while(decimal > 0) {
        int remainder = decimal % 2;
        result = std::to_string(remainder) + result;
        decimal /= 2;
    }
    return result;
}

double EncodeMessage::binaryToDecimal(std::string &binary) {
    int decimal = 0;

    for (int i = 0; i < binary.length(); i++) {
        if (binary[i] == '1') {
            decimal += static_cast<int>(std::pow(2, binary.length() - 1 - i));
        }
    }

    return decimal;
}

bool EncodeMessage::isPrime(int i) {
    if (i == 0 || i == 1) {
        return false;
    }
    for(int a = 2 ; a < i; a++) {
        if(i % a == 0) {
            return false;
        }
    }
    return true;
}
