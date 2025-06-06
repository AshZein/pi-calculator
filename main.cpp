#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <cstdlib>
#include <chrono> // For timing

// Forward declaration
void calculate_pi(mpf_class &pi, unsigned long terms);

int main(int argc, char* argv[]) {
    const unsigned long PRECISION_BITS = 100000;
    const unsigned long DEFAULT_TERMS = 10;

    mpf_set_default_prec(PRECISION_BITS);

    // Get number of terms from argv, or use default
    unsigned long terms = DEFAULT_TERMS;
    if (argc >= 2) {
        terms = std::strtoul(argv[1], nullptr, 10);
        if (terms == 0) {
            std::cerr << "Invalid number of terms: " << argv[1] << "\n";
            return 1;
        }
    } else {
        std::cout << "No term count provided. Using default: " << DEFAULT_TERMS << " terms.\n";
    }

    mpf_class pi;

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();

    calculate_pi(pi, terms);

    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::ofstream out("calculated_outputs/pi_single_output.txt");
    if (out.is_open()) {
        // Calculate the number of accurate digits
        unsigned long accurate_digits = terms * 14;

        // Set the precision for the output stream
        out.precision(accurate_digits);
        out << std::fixed << pi << std::endl;
        out.close();

        std::cout << "π written to pi_single_output.txt using " << terms
                  << " terms, truncated to " << accurate_digits << " digits.\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    // Output the time taken
    std::cout << "Time taken to calculate π: " << duration.count() << " milliseconds.\n";

    return 0;
}
