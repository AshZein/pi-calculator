#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <cstdlib>
#include <chrono> // For timing

#include "chudnovsky.h"
// Forward declaration
void calculate_pi(mpf_class &pi, unsigned long terms);
threaded_calculate_pi(mpf_class &pi, unsigned long terms, int threads);


int main(int argc, char* argv[]) {
    int num_threads = 1; // Default to single-threaded execution
    int total_terms = 10; // Default number of terms
    
    mpf_set_default_prec(PRECISION_BITS);

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d" && i + 1 < argc) {
            total_terms = std::strtoul(argv[++i], nullptr, 10);
        } else if (std::string(argv[i]) == "-t" && i + 1 < argc) {
            num_threads = std::strtoul(argv[++i], nullptr, 10);
        }
    }

    mpf_class pi;

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();
    if (num_threads > 1) {
        std::cout << "Using " << num_threads << " threads to calculate π with" << total_terms << "terms.\n";
        threaded_calculate_pi(pi, total_terms, num_threads);
    } else {
        std::cout << "Calculating π using a single thread with " << total_terms << " terms.\n";
        calculate_pi(pi, terms);
    }
    
    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::ofstream out("calculated_outputs/terms_"<< total_terms << "_threads_"<< num_threads <<"_pi__output.txt");
    if (out.is_open()) {
        // Calculate the number of accurate digits
        unsigned long accurate_digits = terms * 14;

        // Set the precision for the output stream
        out.precision(accurate_digits);
        out << std::fixed << pi << std::endl;
        out.close();

        std::cout << "π written to "calculated_outputs/terms_"<< total_terms << "_threads_"<< num_threads <<"_pi__output.txt".txt using " << terms
                  << " terms, truncated to " << accurate_digits << " digits.\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    // Output the time taken
    std::cout << "Time taken to calculate π: " << duration.count() << " milliseconds.\n";

    return 0;
}
