#include "HexToBinary.h"
#include <string>


std::string HexToBinary::HexToBinary::convert(const std::string& hexStr) {
    std::string binaryStr;
    for (char hexChar : hexStr) {
        unsigned int hexValue;
        std::stringstream ss;
        ss << std::hex << hexChar;
        ss >> hexValue;

        std::bitset<4> binary(hexValue); // Each hex digit represents 4 binary digits
        binaryStr += binary.to_string();
    }
    return binaryStr;
}