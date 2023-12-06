#include "decoding.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <algorithm>  // For std::find
#include <iterator>   // For std::distance
#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
using namespace std;




std::string Decoding::createWindowWithRepeat(const std::string& binaryString) {
    const size_t length1 = 341;
    const size_t length2 = 1023;
    const size_t r1 = 121;  // Additional bits for length1
    const size_t r2 = 77;   // Additional bits for length2

    size_t n = binaryString.length();
    size_t r;
    // Determine the value of r based on the length of the binary string
    if (n == length1) {
        r = r1;
        std::cout << "binary string lenght" << binaryString.length() << std::endl;
        std::cout << "r length" << r << std::endl;
    }
    else if (n == length2) {
        r = r2;
        std::cout << "binary string lenght" << binaryString.length() << std::endl;
        std::cout << "r length" << r << std::endl;
    }
    else {
        // Handle error or unexpected case
        throw std::runtime_error("Invalid binary string length.");
    }
    size_t windowSize = n + r;

    // Start with the binary string and append the first 'r' bits from the original string
    std::string window = binaryString + binaryString.substr(0, r);

    // Ensure the window is exactly n + r bits long
    window = window.substr(0, windowSize);

    return window;
}

//
//std::string Decoding::shiftWindow(std::string& window) {
//    static size_t shiftCounter = 0;  // Static variable to count the number of shifts
//
//    const size_t length1 = 341;
//    const size_t length2 = 1023;
//    const size_t r1 = 121;  // Additional bits for length1
//    const size_t r2 = 77;   // Additional bits for length2
//    size_t n;
//    size_t r;
//    // Check if the shift limit has been reached and adjust the window size
//    if (shiftCounter < 7500) {
//        if (window.length() == length1 + r1) {
//            n = length1;
//            r = r1;
//        }
//        else if (window.length() == length2 + r2) {
//            n = length2;
//            r = r2;
//        }
//        else {
//            std::cerr << "Error: Invalid window length." << std::endl;
//            return ""; // Return an empty string or handle as appropriate
//        }
//    }
//    if (shiftCounter >= 7500) {
//        if (window.length() == length1 + r1 || window.length() == length2 + r2) {
//            n = (window.length() == length1 + r1) ? length1 : length2;
//            r = n;
//
//            // Double the window by appending it to itself
//            window += window;
//        }
//        else {
//            // Handle cases when the window is already doubled
//            n = (window.length() == 2 * length1 || window.length() == 2 * length2) ? window.length() / 2 :
//                (window.length() == length1) ? length1 : length2;
//            r = n;
//        }
//    
//    }
//    
//    // Increment shift counter
//    shiftCounter++;
//    std::cout << shiftCounter << std::endl;
//    std::cout << window.length() << std::endl;
//
//    // Check if window is too short to shift
//    if (window.length() < n + r) {
//        std::cerr << "Error: Window is too short to shift." << std::endl;
//        return ""; // Return an empty string or handle as appropriate
//    }
//
//    // Shift the window by taking elements from index 1 to n
//    std::string shiftedWindow = window.substr(1, n - 1);
//
//    // Append the first r bits of the original binary string to complete the window
//    shiftedWindow += window.substr(0, r + 1);
//
//    return shiftedWindow;
//}

Decoding::Decoding() : hasWindowBeenDoubled(false) {}
std::string Decoding::shiftWindow(std::string& window) {
    static size_t shiftCounter = 0;  // Static variable to count the number of shifts

    const size_t length1 = 341;
    const size_t length2 = 1023;
    const size_t r1 = 121;  // Additional bits for length1
    const size_t r2 = 77;   // Additional bits for length2
    size_t n, r;

    // Increment shift counter
    shiftCounter++;
    /*std::cout << shiftCounter << std::endl;
    std::cout << window.length() << std::endl;*/
    // Check if the shift limit has been reached and adjust the window size
    if (shiftCounter == 7500) {
        n = (window.length() == length1 + r1) ? length1 : length2;
        r = n;
        // Double the window by appending it to itself only once
        if (!hasWindowBeenDoubled) {
            window += window;
            hasWindowBeenDoubled = true;
        }
    }
    else if (shiftCounter < 7500) {
        if (window.length() == length1 + r1) {
            n = length1;
            r = r1;
        }
        else if (window.length() == length2 + r2) {
            n = length2;
            r = r2;
        }
        else {
            std::cerr << "Error: Invalid window length before reaching shift limit." << std::endl;
            return ""; // Return an empty string or handle as appropriate
        }
    }
    else {
        // Handle cases when the window is already doubled
        n = (window.length() == 2 * length1 || window.length() == 2 * length2) ? window.length() / 2 : length1;
        r = n;
    }

    // Check if window is too short to shift
    if (window.length() < n + r) {
        std::cerr << "Error: Window is too short to shift." << std::endl;
        return ""; // Return an empty string or handle as appropriate
    }

    // Shift the window by taking elements from index 1 to n
    std::string shiftedWindow = window.substr(1, n - 1);

    // Append the first r bits of the original binary string to complete the window
    shiftedWindow += window.substr(0, r + 1);

    return shiftedWindow;
}



char Decoding::exor(char a, char b) {
    if (a == b)
        return '0';
    else
        return '1';
}

std::string Decoding::binaryStringXORDivision(std::string dividend, const std::string& divisor) {
    int divisorLength = divisor.length();
    int dividendLength = dividend.length();

    // Extend the dividend by the length of the divisor minus one (for CRC division)
    dividend += std::string(divisorLength - 1, '0');

    std::string rem = dividend.substr(0, divisorLength);

    for (int j = divisorLength; j <= dividend.length(); ++j) {
        if (rem[0] == '1') {
            // Perform XOR with the divisor
            for (int i = 0; i < divisorLength; ++i) {
                rem[i] = exor(rem[i], divisor[i]);
            }
        }

        // Shift remainder and append next bit of dividend
        rem = rem.substr(1, divisorLength - 1) + (j < dividend.length() ? dividend[j] : '0');
    }

    return rem.substr(0, divisorLength - 1);
}



std::string Decoding::processUntilDivisible(const std::string& binaryString, const std::string& gs) {
    std::string window = Decoding::createWindowWithRepeat(binaryString);
    std::string remainder;
    const size_t n = window.length() - ((window.length() == 341 + 121) ? 121 : 77); // Calculate n based on window length

    // Take first n bits of window as the dividend
    std::string dividend = window.substr(0, n);

    // Perform binary XOR division
    remainder = binaryStringXORDivision(dividend, gs);

    // If remainder is zero, the process is complete
    if (remainder == std::string(remainder.size(), '0')) {
        std::cout << "Window length processUntilDivisible: " << window.length() << std::endl;
        return window; // Return the window if divisible
    }

    // Return an empty string if not divisible
    return "";
}

//
std::string Decoding::alignWindow(const std::string& window) {
    const size_t length1 = 341;
    const size_t length2 = 1023;
    const size_t r1 = 121;  // Additional bits for length1
    const size_t r2 = 77;   // Additional bits for length2

    size_t n;
    if (window.length() == length1 + r1) {
        n = length1;
    }
    else if (window.length() == length2 + r2) {
        n = length2;
    }
    else {
        std::cerr << "Warning: Received unexpected window length. Proceeding with best guess." << std::endl;
        n = std::min(window.length(), std::max(length1, length2));
    }

    size_t r = window.length() - n;
    std::string leftMostBits = window.substr(0, r);
    std::string rightMostBits = window.substr(window.length() - r, r);

    if (leftMostBits == rightMostBits) {
        return window; // Return the window if the condition is met
    }

    return ""; // Return an empty string if the condition is not met
}


std::string Decoding::processUntilDivisibleByFs(const std::string& window, const std::string& fs) {
    size_t n = window.length() - ((window.length() == 341 + 121) ? 121 : 77); // Calculate n based on window length

    // Take first n bits of window as the dividend
    std::string dividend = window.substr(0, n);

    // Perform binary XOR division
    std::string remainder = binaryStringXORDivision(dividend, fs);

    // Check if not divisible (remainder is not all zeros)
    if (remainder != std::string(remainder.size(), '0')) {
        return window; // Return the window if not divisible
    }

    return ""; // Return an empty string if divisible
}

std::string Decoding::extractFirstNBits(const std::string& binaryString) {
    size_t n;

    // Determine the value of n based on the length of the binary string
    if (binaryString.length() == 462) {
        n = 341;
    }
    else if (binaryString.length() == 1100) {
        n = 1023;
    }
    else {
        // Handle unexpected string length
        throw std::runtime_error("Invalid binary string length.");
    }

    // Extract the first n bits
    return binaryString.substr(0, n);
}

std::unordered_set<std::string> Decoding::substitutionTable = { "00101","00102","00103","00104","00105","00106","00107","00110","00111","00112",
  "00113","00114","00115","00116","00117","00120","00121","00122","00123","00124",
  "00125","00126","00127","00130","00131","00132","00133","00134","00135","00141",
  "00142","00143","00144","00145","00146","00147","00150","00151","00152","00153",
  "00154","00155","00156","00157","00160","00161","00162","00163","00164","00165",
  "00166","00167","00170","00171","00172","00173","00174","00175","00176","00201",
  "00206","00211","00214","00216","00217","00220","00222","00223","00224","00225",
  "00226","00231","00233","00244","00245","00246","00253","00257","00260","00261",
  "00272","00273","00274","00275","00276","00301","00303","00315","00317","00320",
  "00321","00332","00334","00341","00342","00343","00344","00346","00352","00353",
  "00357","00360","00374","00376","00401","00403","00404","00405","00406","00407",
  "00410","00411","00412","00413","00416","00417","00420","00424","00425","00426",
  "00427","00432","00433","00442","00443","00445","00456","00457","00460","00461",
  "00464","00465","00470","00471","00472","00474","00475","00476","00501","00502",
  "00503","00504","00505","00506","00507","00516","00517","00520","00521","00522",
  "00523","00524","00525","00530","00531","00532","00533","00534","00535","00544",
  "00545","00546","00547","00550","00551","00552","00553","00554","00555","00556",
  "00557","00560","00561","00562","00563","00571","00573","00576","00601","00602",
  "00604","00605","00610","00611","00612","00613","00614","00615","00616","00617",
  "00620","00621","00622","00623","00624","00625","00626","00627","00630","00634",
  "00635","00644","00645","00646","00647","00650","00651","00652","00653","00654",
  "00655","00656","00657","00660","00661","00662","00663","00666","00667","00672",
  "00674","00675","00676","00701","00712","00713","00716","00717","00720","00721",
  "00722","00723","00730","00731","00732","00733","00734","00735","00742","00743",
  "00744","00745","00746","00747","00750","00751","00752","00753","00754","00755",
  "00756","00757","00760","00761","00764","00765","00766","00767","00772","00773",
  "00776","01001","01004","01005","01016","01017","01020","01021","01022","01023",
  "01024","01025","01030","01031","01032","01033","01034","01035","01043","01044",
  "01045","01046","01047","01054","01057","01060","01061","01062","01075","01076",
  "01101","01102","01103","01110","01114","01115","01116","01117","01120","01121",
  "01122","01123","01124","01125","01126","01127","01130","01131","01132","01133",
  "01142","01143","01144","01145","01146","01147","01151","01152","01153","01154",
  "01155","01156","01157","01160","01164","01166","01167","01176","01201","01214",
  "01217","01220","01221","01222","01223","01224","01225","01226","01227","01230",
  "01231","01232","01233","01243","01244","01245","01253","01254","01255","01256",
  "01257","01260","01261","01272","01273","01274","01275","01276","01301","01302",
  "01303","01305","01306","01307","01317","01320","01321","01332","01334","01335",
  "01342","01343","01344","01345","01350","01351","01352","01353","01355","01356",
  "01357","01360","01361","01364","01365","01370","01371","01372","01373","01374",
  "01376","01401","01403","01406","01407","01414","01415","01416","01417","01420",
  "01424","01425","01431","01433","01434","01435","01443","01445","01456","01457",
  "01460","01462","01474","01475","01476","01501","01502","01503","01504","01505",
  "01516","01517","01520","01524","01532","01533","01544","01546","01550","01551",
  "01552","01553","01554","01557","01560","01561","01562","01563","01566","01567",
  "01576","01601","01603","01604","01605","01606","01607","01610","01611","01612",
  "01613","01614","01615","01616","01617","01620","01621","01622","01623","01624",
  "01625","01626","01630","01631","01632","01633","01635","01643","01644","01645",
  "01650","01651","01652","01653","01654","01655","01656","01657","01660","01661",
  "01672","01674","01675","01676","01701","01720","01744","01745","01746","01747",
  "01750","01751","01752","01753","01754","01755","01756","01757","01760","01761",
  "01762","01763","01764","01765","01766","01767","01770","01771","01772","01773",
  "01774","01775","02002","02003","02004","02005","02006","02007","02010","02011",
  "02012","02013","02014","02015","02016","02017","02020","02021","02022","02023",
  "02024","02025","02026","02027","02030","02031","02032","02033","02057","02076",
  "02101","02102","02103","02105","02116","02117","02120","02121","02122","02123",
  "02124","02125","02126","02127","02132","02133","02134","02142","02144","02145",
  "02146","02147","02151","02152","02153","02154","02155","02156","02157","02160",
  "02161","02162","02163","02164","02165","02166","02167","02170","02171","02172",
  "02173","02174","02176","02201","02210","02211","02214","02215","02216","02217",
  "02220","02223","02224","02225","02226","02227","02231","02233","02244","02245",
  "02253","02257","02260","02261","02272","02273","02274","02275","02276","02301",
  "02302","02303","02315","02317","02320","02321","02332","02334","02342","02343",
  "02344","02346","02352","02353","02357","02360","02361","02362","02363","02370",
  "02371","02374","02376","02401","02403","02404","02405","02406","02407","02412",
  "02413","02416","02417","02420","02421","02422","02424","02425","02426","02427",
  "02432","02433","02434","02435","02442","02443","02445","02456","02457","02460",
  "02470","02471","02472","02474","02475","02476","02501","02502","02503","02504",
  "02505","02516","02517","02520","02521","02522","02523","02524","02532","02533",
  "02534","02544","02545","02546","02547","02550","02551","02552","02553","02554",
  "02555","02556","02557","02560","02563","02576","02601","02610","02611","02613",
  "02617","02620","02621","02622","02623","02624","02625","02626","02630","02631",
  "02632","02633","02634","02635","02644","02645","02646","02647","02650","02651",
  "02652","02653","02654","02655","02656","02657","02660","02661","02662","02663",
  "02667","02674","02675","02676","02701","02702","02715","02716","02717","02720",
  "02723","02730","02731","02732","02733","02734","02742","02743","02744","02745",
  "02746","02747","02752","02753","02754","02755","02756","02757","02760","02761",
  "02772","02773","02776","03001","03004","03005","03010","03011","03012","03013",
  "03016","03017","03020","03021","03022","03023","03024","03025","03026","03027",
  "03030","03031","03032","03033","03034","03035","03042","03043","03044","03045",
  "03046","03047","03054","03055","03056","03057","03060","03061","03064","03065",
  "03076","03101","03102","03103","03105","03110","03111","03114","03115","03116",
  "03117","03120","03121","03122","03123","03124","03125","03126","03127","03130",
  "03131","03132","03133","03142","03143","03147","03150","03151","03152","03153",
  "03154","03155","03156","03157","03160","03161","03162","03163","03164","03165",
  "03166","03167","03172","03173","03175","03176","03201","03204","03206","03214",
  "03215","03216","03217","03220","03221","03222","03223","03224","03225","03226",
  "03227","03230","03231","03232","03233","03242","03243","03244","03245","03246",
  "03247","03252","03253","03254","03255","03256","03257","03260","03261","03270",
  "03271","03272","03273","03274","03275","03276","03301","03302","03303","03305",
  "03306","03307","03312","03313","03316","03317","03320","03321","03332","03334",
  "03335","03344","03345","03350","03351","03352","03353","03357","03360","03361",
  "03364","03365","03366","03367","03370","03371","03372","03373","03374","03376",
  "03401","03403","03417","03420","03424","03425","03431","03433","03434","03435",
  "03436","03443","03445","03456","03457","03460","03462","03474","03476","03501",
  "03502","03503","03504","03505","03516","03517","03520","03524","03531","03532",
  "03533","03544","03546","03551","03552","03553","03554","03555","03557","03560",
  "03561","03563","03566","03571","03576","03601","03602","03603","03604","03605",
  "03606","03607","03610","03611","03612","03613","03614","03615","03616","03617",
  "03620","03621","03622","03623","03624","03625","03626","03627","03630","03631",
  "03632","03633","03634","03635","03636","03642","03643","03644","03645","03646",
  "03647","03650","03651","03652","03653","03654","03655","03656","03657","03660",
  "03661","03662","03663","03664","03665","03666","03667","03670","03671","03672",
  "03673","03674","03675","03676" };


std::pair<bool, std::string> Decoding::check11BitWordsAndStore(const std::string& inputString) {
      /*if (binaryString.length() % 11 != 0) {
          std::cerr << "The binary string length is not a multiple of 11." << std::endl;
          return false;
      }*/
     size_t n;

     // Determine the value of n based on the length of the binary string
     if (inputString.length() == 462) {
         n = 341;
     }
     else if (inputString.length() == 1100) {
         n = 1023;
     }
     else {
         // Handle unexpected string length
         throw std::runtime_error("Invalid binary string length.");
     }

     string binaryString = inputString.substr(0, n);

     std::vector<std::string> wordsArray;
     wordsArray.reserve(binaryString.length() / 11);

     for (size_t i = 0; i < binaryString.length(); i += 11) {
          wordsArray.push_back(binaryString.substr(i, 11));
     }

     // Print the elements of wordsArray
     std::cout << "Words Array:" << std::endl;
     for (size_t i = 0; i < wordsArray.size(); ++i) {
          std::cout << "Index " << i << ": " << wordsArray[i] << std::endl;
     }

     std::vector<std::string> octalArray = convertBinaryArrayToOctal(wordsArray);

     std::cout << "octal array size: " << octalArray.size() << std::endl;

     std::cout << "octal array: " << std::endl;
     for (size_t i = 0; i < octalArray.size(); ++i) {
          std::cout << octalArray[i] << std::endl;
     }

      // for (const std::string& octalString : octalArray) {
      //   std::cout << "octalArray" << octalString << std::endl;

      // }

      // Iterate through the octalArray and check each element against the substitution table
     for (size_t i = 0; i < octalArray.size(); ++i) {
         if (substitutionTable.find(octalArray[i]) == substitutionTable.end()) {
              // Element is not present in the substitution table
              std::cerr << "Test failed: Element not found in substitution table - " << octalArray[i]
                  << " at index " << i << std::endl;
              return std::make_pair(false, binaryString);
         }
     }

    // All elements were present in the substitution table
     return std::make_pair(true, binaryString);
}

 // Function to convert an array of binary strings to an array of octal strings
 std::vector<std::string>Decoding::convertBinaryArrayToOctal(const std::vector<std::string>& binaryArray) {
     std::vector<std::string> octalArray;
     for (const std::string& binaryString : binaryArray) {
         try {
             std::string octalString = binaryToOctal(binaryString);
             octalArray.push_back(octalString);
         }
         catch (const std::exception& e) {
             std::cerr << "Error converting binary to octal: " << e.what() << std::endl;
         }
     }
     return octalArray;
 }

 std::string Decoding::binaryToOctal(const std::string& binaryString) {
     // Ensure the binary string is not longer than 11 bits
     if (binaryString.length() > 11) {
         throw std::invalid_argument("The binary string is longer than 11 bits.");
     }

     // Convert binary string to a decimal number
     unsigned long decimalValue = std::bitset<11>(binaryString).to_ulong();

     // Convert decimal number to octal
     std::ostringstream octalStream;
     octalStream << std::oct << decimalValue;

     // Pad the octal number with zeros on the left side to ensure it is 4 digits
     std::string octalString = octalStream.str();
     if (octalString.length() < 5) {
         octalString = std::string(5 - octalString.length(), '0') + octalString;
     }

     return octalString;
 }

 //bool Decoding::checkBitAtPosition109(const std::string& binaryString) {
 //    const size_t targetIndex = 230; // Index of the 109th bit from the right

 //    // Check if the index is within the bounds of the string
 //    if (targetIndex < binaryString.length()) {
 //        return binaryString[targetIndex] == '1';
 //    }
 //    else {
 //        std::cerr << "Binary string is too short to check the 109th bit from the right." << std::endl;
 //        return false; // Or handle this situation as appropriate
 //    }
 //}

 bool Decoding::checkBitAtPosition109(const std::string& binaryString) {
     if (binaryString.length() < 110) {
         std::cerr << "Binary string is too short. It must have at least 110 bits." << std::endl;
         return false;
     }

     // Calculate the position of the bit in the original string
     const size_t originalPosition = binaryString.length() -1 - 109;

     // Check if the bit at the calculated position is '1'
     return binaryString[originalPosition] == '1';
 }

 bool Decoding::checkBitAtPosition108(const std::string& binaryString) {
     if (binaryString.length() < 110) {
         std::cerr << "Binary string is too short. It must have at least 110 bits." << std::endl;
         return false;
     }

     // Calculate the position of the bit in the original string
     const size_t originalPosition = binaryString.length() - 1 - 108;

     // Check if the bit at the calculated position is '1'
     return binaryString[originalPosition] == '1';
 }

 bool Decoding::checkBitAtPosition107(const std::string& binaryString) {
     if (binaryString.length() < 110) {
         std::cerr << "Binary string is too short. It must have at least 110 bits." << std::endl;
         return false;
     }

     // Calculate the position of the bit in the original string
     const size_t originalPosition = binaryString.length() -1 - 107;

     // Check if the bit at the calculated position is '1'
     return binaryString[originalPosition] == '1';
 }

 std::vector<std::string> Decoding::BitTransformation(const std::string& binaryString) {
     std::vector<std::string> wordsArrayBitTransformation;
     std::vector<std::string> octalArrayBitTransformation;
     size_t length = binaryString.length();

     // Calculate the number of bits to consider based on the length of the binary string
     /*size_t n;
     if (length == 462) {
         n = 341;
     }
     else if (length == 1100) {
         n = 1023;
     }
     else {
         throw std::runtime_error("Invalid binary string length.");
     }*/

     /*string binaryString = inputString.substr(0, n);*/

    
     wordsArrayBitTransformation.reserve(binaryString.length() / 11);

     for (size_t i = 0; i < binaryString.length(); i += 11) {
         wordsArrayBitTransformation.push_back(binaryString.substr(i, 11));
     }

     // Print the elements of wordsArray
     std::cout << "Words Array:" << std::endl;
     for (size_t i = 0; i < wordsArrayBitTransformation.size(); ++i) {
         std::cout << "Index " << i << ": " << wordsArrayBitTransformation[i] << std::endl;
     }

    octalArrayBitTransformation = convertBinaryArrayToOctal(wordsArrayBitTransformation);

     std::cout << "octal array size: " << octalArrayBitTransformation.size() << std::endl;

     std::cout << "octal array: " << std::endl;
     for (size_t i = 0; i < octalArrayBitTransformation.size(); ++i) {
         std::cout << octalArrayBitTransformation[i] << std::endl;
     }

     std::vector<int> indices;
     for (const auto& octalValue : octalArrayBitTransformation) {
         bool found = false;
         for (size_t i = 0; i < substitutionTableBitTransformation.size(); ++i) {
             if (substitutionTableBitTransformation[i] == octalValue) {
                 indices.push_back(i);
                 std::cout << "Index of " << octalValue << ": " << i << std::endl;
                 found = true;
                 break;
             }
         }
         if (!found) {
             std::cout << "Octal value " << octalValue << " not found in substitution table." << std::endl;
         }
     }

     std::vector<std::string> tenBitBlocks;

     for (int index : indices) {
         // Convert the integer index to a 10-bit binary string
         std::string binaryBlock = std::bitset<10>(index).to_string();
         tenBitBlocks.push_back(binaryBlock);
     }

     //// Printing the 10-bit blocks
     //std::cout << "10-bit Blocks:" << std::endl;
     //for (const std::string& block : tenBitBlocks) {
     //    std::cout << block << std::endl;
     //}

     return tenBitBlocks; // Now returns an array of octal values
 }

 std::string Decoding::extractFirst210Bits(const std::string& binaryString) {
     const size_t requiredLength = 210;

     if (binaryString.length() < requiredLength) {
         std::cerr << "Error: Binary string is too short. It must have at least 210 bits." << std::endl;
         return ""; // Return an empty string or handle as appropriate
     }

     // Extract the first 210 bits
     return binaryString.substr(0, requiredLength);
 }

 std::vector<std::string> Decoding::substitutionTableBitTransformation = { "00101","00102","00103","00104","00105","00106","00107","00110","00111","00112",
  "00113","00114","00115","00116","00117","00120","00121","00122","00123","00124",
  "00125","00126","00127","00130","00131","00132","00133","00134","00135","00141",
  "00142","00143","00144","00145","00146","00147","00150","00151","00152","00153",
  "00154","00155","00156","00157","00160","00161","00162","00163","00164","00165",
  "00166","00167","00170","00171","00172","00173","00174","00175","00176","00201",
  "00206","00211","00214","00216","00217","00220","00222","00223","00224","00225",
  "00226","00231","00233","00244","00245","00246","00253","00257","00260","00261",
  "00272","00273","00274","00275","00276","00301","00303","00315","00317","00320",
  "00321","00332","00334","00341","00342","00343","00344","00346","00352","00353",
  "00357","00360","00374","00376","00401","00403","00404","00405","00406","00407",
  "00410","00411","00412","00413","00416","00417","00420","00424","00425","00426",
  "00427","00432","00433","00442","00443","00445","00456","00457","00460","00461",
  "00464","00465","00470","00471","00472","00474","00475","00476","00501","00502",
  "00503","00504","00505","00506","00507","00516","00517","00520","00521","00522",
  "00523","00524","00525","00530","00531","00532","00533","00534","00535","00544",
  "00545","00546","00547","00550","00551","00552","00553","00554","00555","00556",
  "00557","00560","00561","00562","00563","00571","00573","00576","00601","00602",
  "00604","00605","00610","00611","00612","00613","00614","00615","00616","00617",
  "00620","00621","00622","00623","00624","00625","00626","00627","00630","00634",
  "00635","00644","00645","00646","00647","00650","00651","00652","00653","00654",
  "00655","00656","00657","00660","00661","00662","00663","00666","00667","00672",
  "00674","00675","00676","00701","00712","00713","00716","00717","00720","00721",
  "00722","00723","00730","00731","00732","00733","00734","00735","00742","00743",
  "00744","00745","00746","00747","00750","00751","00752","00753","00754","00755",
  "00756","00757","00760","00761","00764","00765","00766","00767","00772","00773",
  "00776","01001","01004","01005","01016","01017","01020","01021","01022","01023",
  "01024","01025","01030","01031","01032","01033","01034","01035","01043","01044",
  "01045","01046","01047","01054","01057","01060","01061","01062","01075","01076",
  "01101","01102","01103","01110","01114","01115","01116","01117","01120","01121",
  "01122","01123","01124","01125","01126","01127","01130","01131","01132","01133",
  "01142","01143","01144","01145","01146","01147","01151","01152","01153","01154",
  "01155","01156","01157","01160","01164","01166","01167","01176","01201","01214",
  "01217","01220","01221","01222","01223","01224","01225","01226","01227","01230",
  "01231","01232","01233","01243","01244","01245","01253","01254","01255","01256",
  "01257","01260","01261","01272","01273","01274","01275","01276","01301","01302",
  "01303","01305","01306","01307","01317","01320","01321","01332","01334","01335",
  "01342","01343","01344","01345","01350","01351","01352","01353","01355","01356",
  "01357","01360","01361","01364","01365","01370","01371","01372","01373","01374",
  "01376","01401","01403","01406","01407","01414","01415","01416","01417","01420",
  "01424","01425","01431","01433","01434","01435","01443","01445","01456","01457",
  "01460","01462","01474","01475","01476","01501","01502","01503","01504","01505",
  "01516","01517","01520","01524","01532","01533","01544","01546","01550","01551",
  "01552","01553","01554","01557","01560","01561","01562","01563","01566","01567",
  "01576","01601","01603","01604","01605","01606","01607","01610","01611","01612",
  "01613","01614","01615","01616","01617","01620","01621","01622","01623","01624",
  "01625","01626","01630","01631","01632","01633","01635","01643","01644","01645",
  "01650","01651","01652","01653","01654","01655","01656","01657","01660","01661",
  "01672","01674","01675","01676","01701","01720","01744","01745","01746","01747",
  "01750","01751","01752","01753","01754","01755","01756","01757","01760","01761",
  "01762","01763","01764","01765","01766","01767","01770","01771","01772","01773",
  "01774","01775","02002","02003","02004","02005","02006","02007","02010","02011",
  "02012","02013","02014","02015","02016","02017","02020","02021","02022","02023",
  "02024","02025","02026","02027","02030","02031","02032","02033","02057","02076",
  "02101","02102","02103","02105","02116","02117","02120","02121","02122","02123",
  "02124","02125","02126","02127","02132","02133","02134","02142","02144","02145",
  "02146","02147","02151","02152","02153","02154","02155","02156","02157","02160",
  "02161","02162","02163","02164","02165","02166","02167","02170","02171","02172",
  "02173","02174","02176","02201","02210","02211","02214","02215","02216","02217",
  "02220","02223","02224","02225","02226","02227","02231","02233","02244","02245",
  "02253","02257","02260","02261","02272","02273","02274","02275","02276","02301",
  "02302","02303","02315","02317","02320","02321","02332","02334","02342","02343",
  "02344","02346","02352","02353","02357","02360","02361","02362","02363","02370",
  "02371","02374","02376","02401","02403","02404","02405","02406","02407","02412",
  "02413","02416","02417","02420","02421","02422","02424","02425","02426","02427",
  "02432","02433","02434","02435","02442","02443","02445","02456","02457","02460",
  "02470","02471","02472","02474","02475","02476","02501","02502","02503","02504",
  "02505","02516","02517","02520","02521","02522","02523","02524","02532","02533",
  "02534","02544","02545","02546","02547","02550","02551","02552","02553","02554",
  "02555","02556","02557","02560","02563","02576","02601","02610","02611","02613",
  "02617","02620","02621","02622","02623","02624","02625","02626","02630","02631",
  "02632","02633","02634","02635","02644","02645","02646","02647","02650","02651",
  "02652","02653","02654","02655","02656","02657","02660","02661","02662","02663",
  "02667","02674","02675","02676","02701","02702","02715","02716","02717","02720",
  "02723","02730","02731","02732","02733","02734","02742","02743","02744","02745",
  "02746","02747","02752","02753","02754","02755","02756","02757","02760","02761",
  "02772","02773","02776","03001","03004","03005","03010","03011","03012","03013",
  "03016","03017","03020","03021","03022","03023","03024","03025","03026","03027",
  "03030","03031","03032","03033","03034","03035","03042","03043","03044","03045",
  "03046","03047","03054","03055","03056","03057","03060","03061","03064","03065",
  "03076","03101","03102","03103","03105","03110","03111","03114","03115","03116",
  "03117","03120","03121","03122","03123","03124","03125","03126","03127","03130",
  "03131","03132","03133","03142","03143","03147","03150","03151","03152","03153",
  "03154","03155","03156","03157","03160","03161","03162","03163","03164","03165",
  "03166","03167","03172","03173","03175","03176","03201","03204","03206","03214",
  "03215","03216","03217","03220","03221","03222","03223","03224","03225","03226",
  "03227","03230","03231","03232","03233","03242","03243","03244","03245","03246",
  "03247","03252","03253","03254","03255","03256","03257","03260","03261","03270",
  "03271","03272","03273","03274","03275","03276","03301","03302","03303","03305",
  "03306","03307","03312","03313","03316","03317","03320","03321","03332","03334",
  "03335","03344","03345","03350","03351","03352","03353","03357","03360","03361",
  "03364","03365","03366","03367","03370","03371","03372","03373","03374","03376",
  "03401","03403","03417","03420","03424","03425","03431","03433","03434","03435",
  "03436","03443","03445","03456","03457","03460","03462","03474","03476","03501",
  "03502","03503","03504","03505","03516","03517","03520","03524","03531","03532",
  "03533","03544","03546","03551","03552","03553","03554","03555","03557","03560",
  "03561","03563","03566","03571","03576","03601","03602","03603","03604","03605",
  "03606","03607","03610","03611","03612","03613","03614","03615","03616","03617",
  "03620","03621","03622","03623","03624","03625","03626","03627","03630","03631",
  "03632","03633","03634","03635","03636","03642","03643","03644","03645","03646",
  "03647","03650","03651","03652","03653","03654","03655","03656","03657","03660",
  "03661","03662","03663","03664","03665","03666","03667","03670","03671","03672",
  "03673","03674","03675","03676" };

//string initializeLFSR(const std::string& binaryString) {
//    // Convert the binary string to a uint32_t value
//
//    return lfsrSeed = binaryString;
//}
bool Decoding::clockLFSR(std::string& lfsrSeed) {
    // Manually perform XOR on the binary string
    bool newBit = (lfsrSeed[0] ^ lfsrSeed[1] ^ lfsrSeed[2] ^
        lfsrSeed[4] ^ lfsrSeed[6] ^ lfsrSeed[31]) & 1;
    lfsrSeed = lfsrSeed.substr(1) + (newBit ? '1' : '0');
    return newBit;
}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    const unsigned int modValue = 1 << blockSize; // 2^10
//    int numBlocks = data.size() / blockSize;
//
//    // Compute the sum of all 10-bit blocks except the first one
//    int sumExceptFirst = 0;
//    for (int i = 1; i < numBlocks; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sumExceptFirst += std::bitset<10>(block).to_ulong();
//    }
//
//    // Get the current (modified) value of the first block
//    int modifiedFirstBlockValue = std::bitset<10>(data.substr(0, blockSize)).to_ulong();
//
//    // Compute the original value of the first block
//    int originalFirstBlockValue = (modifiedFirstBlockValue + modValue - sumExceptFirst % modValue) % modValue;
//
//    // Replace the first 10 bits
//    std::bitset<10> originalFirstBlockBits(originalFirstBlockValue);
//    data.replace(0, blockSize, originalFirstBlockBits.to_string());
//
//    cout << "descrambled correct first 10 bits: " << data << endl;
//
//}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    int numBlocks = data.size() / blockSize;
//
//    unsigned int sum = 0;
//
//    // Calculate sum(U(k-2..0)):
//    for (int i = 0; i < numBlocks - 1; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sum += std::bitset<10>(block).to_ulong() & 0x3FF;  // Get word and accumulate sum
//    }
//
//    // Calculate U(k-1) and save it in data:
//    std::string lastBlock = data.substr((numBlocks - 1) * blockSize, blockSize);
//    unsigned int lastBlockValue = std::bitset<10>(lastBlock).to_ulong();
//    unsigned int correctedValue = lastBlockValue - sum;
//
//    // Replace the last 10 bits (U(k-1)) with the corrected value
//    std::bitset<10> correctedBits(correctedValue);
//    data.replace((numBlocks - 1) * blockSize, blockSize, correctedBits.to_string());
//
//    std::cout << "Corrected first 10 bits: " << data << std::endl;
//}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    const int modValue = 1024; // 2^10
//    int numBlocks = data.size() / blockSize;
//
//    unsigned int sum = 0;
//
//    // Calculate sum(Ui) for i = 0 to k-1
//    for (int i = 0; i < numBlocks; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sum += std::bitset<10>(block).to_ulong();
//    }
//
//    // Apply modulo 2^10 to the sum
//    sum %= modValue;
//
//    // Convert the sum back to a 10-bit binary string
//    std::string correctedFirstBlock = std::bitset<10>(sum).to_string();
//
//    
//
//    // Replace the first block (Uk-1) with the corrected value
//    data.replace(0, blockSize, correctedFirstBlock);
//
//    std::cout << "Corrected first 10 bits: " << correctedFirstBlock << std::endl;
//}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    int numBlocks = data.size() / blockSize;
//
//    // Extract the first 10 bits (U'(k-1))
//    std::string firstBlock = data.substr(0, blockSize);
//    unsigned int uPrimeKMinus1 = std::bitset<10>(firstBlock).to_ulong();
//
//    unsigned int sumExceptFirst = 0;
//
//    // Calculate sum(Ui) for i = 0 to k-2
//    for (int i = 1; i < numBlocks; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sumExceptFirst += std::bitset<10>(block).to_ulong();
//    }
//
//    // Calculate U(k-1) = U'(k-1) - sum(Ui for i=0 to k-2)
//    unsigned int correctedValue = uPrimeKMinus1 - sumExceptFirst;
//
//    // Convert the corrected value back to a 10-bit binary string
//    std::bitset<10> correctedBits(correctedValue);
//
//    // Replace the first block (U(k-1)) with the corrected value
//    data.replace(0, blockSize, correctedBits.to_string());
//
//    std::cout << "Corrected first 10 bits: " << correctedBits << std::endl;
//}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    int numBlocks = data.size() / blockSize;
//
//    // Extract the first 10 bits (U'(k-1))
//    std::string firstBlock = data.substr(0, blockSize);
//    uint32_t uPrimeKMinus1 = std::bitset<10>(firstBlock).to_ulong();  // Using uint32_t
//
//    cout << "uPrimekMinus1" << uPrimeKMinus1 << endl;
//
//    uint32_t sumExceptFirst = 0;  // Also using uint32_t here
//
//    // Calculate sum(Ui) for i = 0 to k-2
//    for (int i = 1; i < numBlocks; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sumExceptFirst += std::bitset<10>(block).to_ulong();
//    }
//
//    cout<< "sumExceptFirst: " << sumExceptFirst << endl;
//
//    // Calculate U(k-1) = U'(k-1) - sum(Ui for i=0 to k-2)
//    uint32_t correctedValue = uPrimeKMinus1 - sumExceptFirst;
//
//    // Convert the corrected value back to a 10-bit binary string
//    std::bitset<10> correctedBits(correctedValue);
//
//    // Replace the first block (U(k-1)) with the corrected value
//    data.replace(0, blockSize, correctedBits.to_string());
//
//    std::cout << "Corrected first 10 bits: " << correctedBits.to_string() << std::endl;
//}


string Decoding::correctFirst10Bits(std::string& data) {
    const int blockSize = 10;
    const uint32_t modValue = 1024; // 2^10
    int numBlocks = data.size() / blockSize;

    std::string firstBlock = data.substr(0, blockSize);
    cout << "first block: " << firstBlock << endl;
    uint32_t uPrimeKMinus1 = std::bitset<10>(firstBlock).to_ulong();
   
    cout << "uPrimeKMinus1: " << uPrimeKMinus1 << endl;

    uint32_t sumExceptFirst = 0;
    for (int i = 1; i < numBlocks; ++i) {
        std::string block = data.substr(i * blockSize, blockSize);
       /* cout << "block: " << block[i] << endl;*/
        sumExceptFirst += std::bitset<10>(block).to_ulong();
    }

    

    sumExceptFirst = sumExceptFirst & 0x3FF;  // Apply modulo to sumExceptFirst
    cout << "sum: " << sumExceptFirst << endl;

    // Handle underflow in subtraction
    /*uint32_t correctedValue = (uPrimeKMinus1 - sumExceptFirst);*/

    uint32_t correctedValue = (uPrimeKMinus1 - sumExceptFirst);

    cout << "corrected value decimal: " << correctedValue << endl;


    std::bitset<10> correctedBits(correctedValue);
    data.replace(0, blockSize, correctedBits.to_string());

    std::cout << "Corrected first 10 bits: " << correctedBits << std::endl;
   

    return data;
}

//void Decoding::correctFirst10Bits(std::string& data) {
//    const int blockSize = 10;
//    int numBlocks = data.size() / blockSize;
//
//    // Extract U'(k-1)
//    std::string uPrimeKMinus1Str = data.substr(0, blockSize);
//    uint32_t uPrimeKMinus1 = std::bitset<10>(uPrimeKMinus1Str).to_ulong();
//
//    uint32_t sumOfRest = 0;
//
//    // Sum the rest of the blocks U'(k-2) to U'(0)
//    for (int i = 1; i < numBlocks; ++i) {
//        std::string block = data.substr(i * blockSize, blockSize);
//        sumOfRest += std::bitset<10>(block).to_ulong();
//    }
//
//    // Calculate U(k-1) = (U'(k-1) - sumOfRest) mod 2^10
//    uint32_t uKMinus1 = (uPrimeKMinus1 - sumOfRest); // 1024 = 2^10
//
//    // Convert U(k-1) back to a 10-bit binary string
//    std::string uKMinus1Str = std::bitset<10>(uKMinus1).to_string();
//
//    /*std::bitset<10> correctedBits(uKMinus1Str);*/
//    data.replace(0, blockSize, uKMinus1Str);
//
//
//    
//    std::cout << "Corrected first 10 bits: " << uKMinus1Str << std::endl;
//
//};


void Decoding::calc_first_word(t_longnum& U, int m) {
    unsigned int sum = 0, temp, i;

    for (i = 0; i <= (m / 10 - 2); i++) {
        sum += long_get_word(U, i * 10) & 0x3FF;
    }

    temp = long_get_word(U, m - 10) - sum;
    long_write_at_location(U, m - 10, &temp, 10);
}

using t_word = uint32_t;
t_word Decoding::long_get_word(const t_longnum& longnum, int bitnum) {
    int word_index, bit_index;
    t_word retval = 0;

    if ((bitnum >= BITS_IN_LONGNUM) || (bitnum < 0)) {
        return 0;
    }

    word_index = bitnum / BITS_IN_WORD;
    bit_index = bitnum % BITS_IN_WORD;

    retval = (longnum[word_index] >> bit_index);

    if ((bit_index > 0) && (word_index < WORDS_IN_LONGNUM - 1)) {
        retval |= (longnum[word_index + 1] << (BITS_IN_WORD - bit_index));
    }

    return retval;
}
//
void Decoding::long_write_at_location(t_longnum& longnum, int location, t_word* newvalue, int n_bits) {
    if ((location >= BITS_IN_LONGNUM) || (location < 0)) {
        return;
    }

    if (location + n_bits > BITS_IN_LONGNUM) {
        n_bits = BITS_IN_LONGNUM - location;
    }

    for (int i = 0; i < n_bits; i++) {
        int n_word = i / BITS_IN_WORD;
        int n_bit = i % BITS_IN_WORD;

        long_setbit(longnum, location + i, (newvalue[n_word] >> n_bit) & 1);
    }
}

void Decoding::long_setbit(t_longnum& longnum, int bitnum, int value) {
    if ((bitnum < 0) || (bitnum >= BITS_IN_LONGNUM)) {
        return;
    }

    int word_index = bitnum / BITS_IN_WORD;
    int bit_index = bitnum % BITS_IN_WORD;

    t_word mask = static_cast<t_word>(1) << bit_index;

    if (value) {
        longnum[word_index] |= mask;
    }
    else {
        longnum[word_index] &= ~mask;
    }
}


//std::string Decoding::descramble(const std::string& scrambledData) {
//    // Calculate B from the given bits and initialize LFSR
//    uint32_t B = (0*2^11+0*2^10+0*2^9+0*2^8+0*2^7+0*2^6+0*2^5+1*2^4+0*2^3+1*2^2+1*2^1+1);
//    /*uint32_t S = (2801775573u * B) % (1u << 32);*/
//    uint32_t S = 2801775573u * B;
//
//    // Convert S to its binary representation
//    std::string binaryS = std::bitset<32>(S).to_string();
//    /*initializeLFSR(binaryS);*/
//
//    std::string descrambledData;
//    for (char bit : scrambledData) {
//        bool lfsrOutputBit = clockLFSR(binaryS);
//        descrambledData.push_back((bit - '0' ^ lfsrOutputBit) + '0');
//    }
//
//    correctFirst10Bits(descrambledData);
//    return descrambledData;
//}

std::string Decoding::descramble(const std::string& scrambledData, const std::string& safeTelegram) {
    // Initialize constants and LFSR
    static const uint32_t H = 1 << 31 | 1 << 30 | 1 << 29 | 1 << 27 | 1 << 25 | 1;
    //uint32_t B = ( 1 *pow(2,11) + 1 *pow(2,10) +1*pow(2,8) + pow(2,7)+pow(2,6)+pow(2,2));
    uint32_t B = extractBits(safeTelegram);
    uint32_t S = 2801775573u * B;

    std::string descrambledData;
    descrambledData.reserve(scrambledData.size());  // Reserve for efficiency

    for (char bit : scrambledData) {
        char scrambled_bit = bit - '0';  // Convert char to bit
        char t = static_cast<char>(S >> 31);  // Get current output of the shift register
        char descrambled_bit = t ^ scrambled_bit;  // XOR with LFSR output

        // Append the descrambled bit to the result string
        descrambledData.push_back(descrambled_bit + '0');

        // Shift the register and apply H if needed
        S <<= 1;
        if (scrambled_bit) {
            S ^= H;
        }
    }
    cout << "data after reversing lfsr: " << descrambledData << endl;

    string finalData = correctFirst10Bits(descrambledData);
  
    return finalData;
}

//std::string Decoding::descramble(const std::string& scrambledData) {
//    static const uint32_t H = (1 << 31) | (1 << 30) | (1 << 29) | (1 << 27) | (1 << 25) | 1;
//    uint32_t B = 0b000000100111;  // Assuming this is the correct binary representation
//    uint32_t S = 2801775573u * B;
//
//    std::string descrambledData;
//    descrambledData.reserve(scrambledData.size());
//
//    // Descramble process
//    for (char bit : scrambledData) {
//        char scrambled_bit = bit - '0';
//        char t = static_cast<char>(S >> 31);
//        char descrambled_bit = t ^ scrambled_bit;
//        descrambledData.push_back(descrambled_bit + '0');
//        S <<= 1;
//        if (scrambled_bit) {
//            S ^= H;
//        }
//    }
//
//    // Convert string to t_longnum
//    t_longnum longnumData = stringToLongnum(descrambledData);
//
//    
//
//    // Correct the first 10 bits using calc_first_word
//    calc_first_word(longnumData, descrambledData.size());
//
//    string finalData = longnumToString(longnumData);
//    // Convert t_longnum back to string
//    return finalData;
//}
//using t_longnum = std::vector<uint32_t>;
//t_longnum Decoding::stringToLongnum(const std::string& binaryString) {
//    t_longnum longnum(BITS_IN_LONGNUM / BITS_IN_WORD, 0);
//    // Conversion from binary string to longnum format
//    for (size_t i = 0; i < binaryString.size(); ++i) {
//        if (binaryString[i] == '1') {
//            long_setbit(longnum, i, 1);
//        }
//    }
//    return longnum;
//}

std::string Decoding::longnumToString(const t_longnum& longnum) {
    std::string binaryString;
    binaryString.reserve(BITS_IN_LONGNUM);
    for (int i = 0; i < BITS_IN_LONGNUM; ++i) {
        binaryString.push_back((long_get_word(longnum, i) & 1) ? '1' : '0');
    }
    return binaryString;
}




std::vector<std::string> Decoding::extractFirst21Blocks(const std::vector<std::string>& binaryBlocks) {
    std::vector<std::string> extractedBlocks;
    const size_t numberOfBlocks = 21;

    for (size_t i = 0; i < numberOfBlocks && i < binaryBlocks.size(); ++i) {
        extractedBlocks.push_back(binaryBlocks[i]);
    }

    return extractedBlocks;
}

std::string Decoding::concatenateStringVector(const std::vector<std::string>& stringVector) {
    std::string concatenatedString;

    for (const auto& block : stringVector) {
        concatenatedString += block;
    }

    return concatenatedString;
}
uint32_t Decoding::extractBits(const std::string& binaryString) {
    std::string extractedBits;

    // The bit positions you want to extract
    int positions[] = { 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95 };

    // Iterate over the positions array
    for (int pos : positions) {
        // Calculate the index in the string based on the provided indexing
        int index = binaryString.length() - 1 - pos;

        // Ensure the index is within the bounds of the string
        if (index >= 0 && index < binaryString.length()) {
            extractedBits += binaryString[index];
        }
        else {
            std::cerr << "Index out of bounds." << std::endl;
            return 0 ; // Or handle this situation as appropriate
        }
    }
    std::bitset<12> bitset(extractedBits); // Assuming 12 bits as per the positions array
    return bitset.to_ulong();
    /*return extractedBits;*/
}

int Decoding::binaryToDecimal(const std::string& binaryStr) {
    return std::bitset<32>(binaryStr).to_ulong();
}

// Main function to decode the binary string
void Decoding::decodeBinaryString(const std::string& binaryStr) {
    // Define the sequence of message codes, their names, and bit lengths
    std::vector<std::pair<std::string, int>> messageCodes = {
        {"Q_UPDOWN", 1}, {"M_VERSION", 7}, {"Q_MEDIA", 1}, {"N_PIG", 3},
        {"N_TOTAL", 3}, {"M_DUP", 2}, {"M_MCOUNT", 8}, {"NID_C", 10},
        {"NID_BG", 14}, {"Q_LINK", 1}, {"NID_PACKET", 8}, {"Q_DIR", 2},
        {"L_PACKET", 13}, {"Q_SCALE", 2}, {"NID_TSR", 8}, {"D_TSR", 15},
        {"L_TSR", 15}, {"Q_FRONT", 1}, {"V_TSR", 7}, {"NID_PACKET_2", 8},
        {"Q_DIR_2", 2}, {"L_PACKET_2", 13}, {"Q_SRSTOP", 1}, {"NID_PACKET_3", 8}
        // Add other message codes as needed
    };

    int currentIndex = 0;

    for (const auto& [codeName, length] : messageCodes) {
        // Extract the binary subsequence
        std::string subBinary = binaryStr.substr(currentIndex, length);

        // Convert the binary subsequence to decimal
        int decimalValue = binaryToDecimal(subBinary);

        // Output the message code name and its decimal value
        std::cout << codeName << ": " << decimalValue << std::endl;

        // Move to the next subsequence
        currentIndex += length;
    }
}
