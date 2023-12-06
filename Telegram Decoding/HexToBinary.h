#pragma once
#ifndef HEXTOBINARY_H
#define HEXTOBINARY_H

#include <string>
#include <sstream>
#include <bitset>

class HexToBinary {
public:
    static std::string convert(const std::string& hexStr);
};

#endif // HEXTOBINARY_H