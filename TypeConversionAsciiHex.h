#pragma once
// TypeConversion.h

#ifndef TYPECONVERSIONASCIIHEX_H
#define TYPECONVERSIONASCIIHEX_H

#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <cmath> // For std::abs

class TypeConversionAsciiHex {
public:
    static std::string readFile(const std::string& filename);
    /*td::string asciiToHex(const char& c);*/
    static std::string asciiToHexString(const std::string& asciiStr);
    static std::string HexToBin(const std::string& hexStr);
    static std::string adjustBinaryStringLength(const std::string& binaryString);

};

#endif // TYPECONVERSION_H