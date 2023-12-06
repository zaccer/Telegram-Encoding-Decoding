#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "TypeConversionAsciiHex.h"

//std::string TypeConversionAsciiHex::readFile(const std::string& filename) {
//    std::ifstream file(filename, std::ios::binary);
//    std::string data;
//
//    if (file.is_open()) {
//        std::getline(file, data, '\0');
//        file.close();
//    }
//    else {
//        std::cerr << "Unable to open file." << std::endl;
//    }
//
//    return data;
//}

//std::string TypeConversionAsciiHex::asciiToHex(const char& c) {
//    std::stringstream ss;
//    ss << std::hex << std::uppercase << static_cast<int>(static_cast<unsigned char>(c));
//    return ss.str();
//}
std::string TypeConversionAsciiHex::asciiToHexString(const std::string& asciiStr) {
    std::stringstream ss;
    for (char c : asciiStr) {
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
    }
    return ss.str();
}
std::string TypeConversionAsciiHex::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }
    else {
        std::cerr << "Unable to open file." << std::endl;
        return "";
    }
}
std::string TypeConversionAsciiHex::HexToBin(const std::string& hexStr) {
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
//std::string TypeConversionAsciiHex::adjustBinaryStringLength(const std::string& binaryString) {
//    const size_t length1 = 341;
//    const size_t length2 = 1023;
//    size_t binaryLength = binaryString.length();
//
//    // No adjustment needed if the length is already one of the desired lengths
//    if (binaryLength == length1 || binaryLength == length2) {
//        return binaryString;
//    }
//
//    size_t newLength = (std::abs(int(binaryLength - length1)) < std::abs(int(binaryLength - length2))) ? length1 : length2;
//
//    if (binaryLength > newLength) {
//        return binaryString.substr(binaryLength - newLength);
//    }
//    else {
//        // If the binary string is shorter than the desired length, return as is
//        // Or consider padding the string to the desired length
//        return binaryString;
//    }
//}

std::string TypeConversionAsciiHex::adjustBinaryStringLength(const std::string& binaryString) {
    const size_t length1 = 341;
    const size_t length2 = 1023;
    size_t binaryLength = binaryString.length();

    // No adjustment needed if the length is already one of the desired lengths
    if (binaryLength == length1 || binaryLength == length2) {
        return binaryString;
    }

    size_t newLength = (std::abs(int(binaryLength - length1)) < std::abs(int(binaryLength - length2))) ? length1 : length2;

    if (binaryLength > newLength) {
        // Take a substring from the start, with the length of newLength
        return binaryString.substr(0, newLength);
    }
    else {
        // If the binary string is shorter than the desired length, return as is
        // Or consider padding the string to the desired length
        return binaryString;
    }
}