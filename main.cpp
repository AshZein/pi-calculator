#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "chudnovsky.cpp"

int main(int argc, char* argv[]) {
    // Default precision
    unsigned long precision = 100;

    // Check if user provided a precision argument
    if (argc > 1) {
        try {
            precision = std::stoul(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid input. Using default precision of 100 digits." << std::endl;
        }
    }

    // Calculate pi
    mpf_class pi;
    calculate_pi(pi, precision);

    // Output the result
    std::cout << "Calculated value of pi to " << precision << " digits:" << std::endl;
    std::cout.precision(precision);
    std::cout << pi << std::endl;

    return 0;
}