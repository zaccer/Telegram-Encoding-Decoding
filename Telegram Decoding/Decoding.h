#pragma once
#ifndef DECODING_H
#define DECODING_H

#include <string>
#include <unordered_set>
#include <cstdint>

class Decoding {
private:
    bool hasWindowBeenDoubled;
    uint32_t lfsr;
    

    uint32_t lfsrSeed;
    std::string lfsrScrambledData;
    /*void initializeLFSR(const std::string& binaryString);*/
    bool clockLFSR(std::string& lfsrSeed);
    
    

public:
    Decoding();
    using t_longnum = std::vector<uint32_t>;
    using t_word = uint32_t;

    static const int BITS_IN_WORD = 32;  // Assuming each word is 32 bits
    static const int BITS_IN_LONGNUM = 1024;  // Assuming the total number of bits in t_longnum
    static const int WORDS_IN_LONGNUM = BITS_IN_LONGNUM / BITS_IN_WORD;  // Total words in t_longnum

    static std::string createWindowWithRepeat(const std::string& binaryString);
    static std::string binaryStringXORDivision(std::string dividend,const std::string& divisor);
    
    static char exor(char a, char b);
    std::string shiftWindow(std::string& window);
    static std::string processUntilDivisible(const std::string& binaryString, const std::string& gs);
    std::string alignWindow(const std::string& window);
    static std::string processUntilDivisibleByFs(const std::string& window, const std::string& fs);
    static std::string extractFirstNBits(const std::string& binaryString);
    static std::unordered_set<std::string> substitutionTable;
    static std::vector<std::string> substitutionTableBitTransformation;
    static std::pair<bool, std::string> check11BitWordsAndStore(const std::string& binaryString);
    static std::vector<std::string> convertBinaryArrayToOctal(const std::vector<std::string>& binaryArray);
    static std::string binaryToOctal(const std::string& binaryString);
    static bool checkBitAtPosition109(const std::string& binaryString);
    static bool checkBitAtPosition108(const std::string& binaryString);
    static bool checkBitAtPosition107(const std::string& binaryString);
    static std::string extractFirst210Bits(const std::string& binaryString);
    static std::vector < std::string> BitTransformation(const std::string & binaryString);
    static std::vector<std::string> extractFirst21Blocks(const std::vector<std::string>& binaryBlocks);

    static std::string descramble(const std::string& scrambledData, const std::string& safeTelegram);
    static std::string correctFirst10Bits(std::string& data);

    static std::string concatenateStringVector(const std::vector<std::string>& stringVector);
    static uint32_t extractBits(const std::string& binaryString);

    static int binaryToDecimal(const std::string& binaryStr);

    static void decodeBinaryString(const std::string& binaryStr);
    

    static void calc_first_word(t_longnum& U, int m);
    static t_word long_get_word(const t_longnum& longnum, int bitnum);
    static void long_write_at_location(t_longnum& longnum, int location, t_word* newvalue, int n_bits);
    static void long_setbit(t_longnum& longnum, int bitnum, int value);
    t_longnum stringToLongnum(const std::string& binaryString);
    std::string longnumToString(const t_longnum& longnum);

};

#endif // DECODING_H