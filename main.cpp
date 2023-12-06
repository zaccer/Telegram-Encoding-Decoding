#include <string>
#include "TypeConversionAsciiHex.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "../../Telegram Encoding/Telegram Decoding/Decoding.h"
using namespace std;

int main() {
    TypeConversionAsciiHex conversion;
    Decoding decoding;
   
    string gs = "1001111101111001000011000010111111101111011111001010010010100011110001001011";
    string fs = "10110101011";
    string filename = "felix_3.tlg";
    string data = conversion.readFile(filename);

    string hexString = conversion.asciiToHexString(data);
    cout << "ASCII to Hex: " << hexString << endl;

    string binaryString = conversion.HexToBin(hexString);
    cout << "Binary: " << binaryString << "\n";
    cout << "Original Length: " << binaryString.length() << endl;

    string adjustedBinaryString = conversion.adjustBinaryStringLength(binaryString);
    std::string finalBinaryString;
    


    while (true) {
        string result;

        // First check: processUntilDivisible
        result = decoding.processUntilDivisible(adjustedBinaryString, gs);
        if (result.empty()) {
            adjustedBinaryString = decoding.shiftWindow(adjustedBinaryString);
            continue;
        }
        else {
            adjustedBinaryString = result;
        }

        // Second check: alignWindow
        result = decoding.alignWindow(adjustedBinaryString);
        if (result.empty()) {
            adjustedBinaryString = decoding.shiftWindow(adjustedBinaryString);
            continue;
        }
        else {
            adjustedBinaryString = result;
        }

        // Third check: processUntilDivisibleByFs
        result = decoding.processUntilDivisibleByFs(adjustedBinaryString, fs);
        if (result.empty()) {
            adjustedBinaryString = decoding.shiftWindow(adjustedBinaryString);
            cout << "shift" << endl;
            continue;
        }
        else {
            adjustedBinaryString = result;
        }

        // Fourth check: check11BitWordsAndStore
        auto [checkPassed, binaryString] = decoding.check11BitWordsAndStore(adjustedBinaryString);
        if (!checkPassed) {
            adjustedBinaryString = decoding.shiftWindow(adjustedBinaryString);
            std::cout << "shift" << std::endl;
            continue;
        }
        else {
            finalBinaryString = binaryString; // Save the binary string
            std::cout << "Telegram is safe" << std::endl;
            break; // All checks passed, exit the loop
        }
    }

    /*cout << "telegram: " << finalBinaryString << endl;
    cout << "telegram length: " << finalBinaryString.length() << endl;*/

    /*bool result = decoding.checkBitAtPosition109(finalBinaryString);

    cout << "The bit at position 109 from the right is " << (result ? "1" : "0") << endl;

    bool result_108 = decoding.checkBitAtPosition108(finalBinaryString);

    cout << "The bit at position 108 from the right is " << (result_108 ? "1" : "0") << endl;

    bool result_107 = decoding.checkBitAtPosition107(finalBinaryString);

    cout << "The bit at position 107 from the right is " << (result_107 ? "1" : "0") << endl;*/

    /*string userBits = decoding.extractFirst210Bits(finalBinaryString);

    cout << "user bits length: " << userBits.length() << endl;*/


    auto binArray = Decoding::BitTransformation(finalBinaryString);
    

    // Printing the octal values
    for (const auto& binBlock : binArray) {
        std::cout << binBlock << std::endl;
    }
    /*cout << "length of octal array: " << binArray.size() << endl;*/

    auto blocks = decoding.extractFirst21Blocks(binArray);

    // Output the extracted blocks
    for (size_t i = 0; i < blocks.size(); ++i) {
        std::cout << "Block " << i + 1 << ": " << blocks[i] << std::endl;
    }

    string userBits = decoding.concatenateStringVector(blocks);
    /*cout << "user bits: " << userBits << endl;
    cout << "user bits length: " << userBits.length() << endl;*/

    string descrambledData = decoding.descramble(userBits,finalBinaryString);

    //cout << "descramble data: " << descrambledData << endl;
    //cout << "descramble data length: " << descrambledData.length() << endl;

    decoding.decodeBinaryString(descrambledData);

    cout << "Press enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any previous input
    cin.get(); // Wait for enter key

    return 0;
}


