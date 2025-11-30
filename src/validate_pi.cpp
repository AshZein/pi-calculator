#include "validate_pi.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

// Compare two digit files character-by-character and report first mismatch.
// Returns:
//   - number of matching digits (uint64_t)
//   - or -1 on file-open error
//   - and prints detailed mismatch info
//
// Notes:
//   - Skips non-digit characters in both files (handles "3.", newlines)
//   - Streams bytes, no large memory usage
//
long long validate_pi_files(const std::string &myFile, const std::string &refFile) {
    std::ifstream mine(myFile);
    std::ifstream ref(refFile);

    if (!mine || !ref) {
        std::cerr << "Error: Could not open input files.\n";
        return -1;
    }

    char a, b;
    uint64_t index = 0;

    std::string currentDigitMine = "";
    std::string currentDigitRef = "";

    // Helper to get next digit from any stream
    auto nextDigit = [](std::ifstream &f, char &c) -> bool {
        while (f.get(c)) {
            if (std::isdigit(static_cast<unsigned char>(c)))
                return true;
        }
        return false; // EOF
    };

    while (true) {
        bool okA = nextDigit(mine, a);
        bool okB = nextDigit(ref, b);

        // If both streams reached EOF → digits matched up to this point
        if (!okA || !okB) {
            std::cout << "Comparison finished. Files match for "
                      << index << " digits.\n";
            return index;
        }

        // reset buffers
        if(currentDigitMine.length() == 50){
            currentDigitMine.erase(0, 1);
            currentDigitRef.erase(0, 1);
        }

        // add to current buffers
        currentDigitMine += a;
        currentDigitRef += b;

        index++;

        if (a != b) {
            std::cout << "\nMismatch at digit #" << index
                      << "   mine=" << a << "   ref=" << b << "\n";
            
            std::cout << "mine:\n" << currentDigitMine << "\n\nref:\n" << currentDigitRef << "\n\n";
        
            return index;  // return where mismatch occurred
        }
    }

    // unreachable, but included for completeness
    return index;
}
