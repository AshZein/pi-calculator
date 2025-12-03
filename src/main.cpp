#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <cstdlib>
#include <chrono> // For timing

#include "chudnovsky.h"
#include "validate_pi.h"
#include "binary_split_chudnovsky.h"

// Forward declaration
void calculate_pi(mpf_class &pi, unsigned long terms);
void threaded_calculate_pi(mpf_class &pi, unsigned long terms, int threads);


int main(int argc, char* argv[]) {
    int num_threads = 1; // Default to single-threaded execution
    unsigned long total_terms = 10; // Default number of terms
    bool verify = true;
    bool binarySplit = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d" && i + 1 < argc) {
            total_terms = std::strtoul(argv[++i], nullptr, 10);
        } else if (std::string(argv[i]) == "-t" && i + 1 < argc) {
            num_threads = std::strtoul(argv[++i], nullptr, 10);
        } else if (std::string(argv[i]) == "-v" && i + 1 < argc){
            verify = false;
        } else if (std::string(argv[i]) == "-b"){
            binarySplit = true;
        }
    }

    unsigned long desired_digits = total_terms * 14;
    unsigned long bits = static_cast<unsigned long>(desired_digits * 3.32193) + 1000; // add margin
    mpf_set_default_prec(bits);

    mpf_class pi;

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();
    if (binarySplit){
        std::cout << "Using binary splitting to calculate pi with " << total_terms << " terms.\n";
        binarySplitPi(pi, total_terms);
    }else if (num_threads > 1) {
        std::cout << "Using " << num_threads << " threads to calculate π with" << total_terms << "t erms.\n";
        threaded_calculate_pi(pi, total_terms, num_threads);
    } else {
        std::cout << "Calculating π using a single thread with " << total_terms << " terms.\n";
        calculate_pi(pi, total_terms);
    }
    
    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Build the filename first
    std::string filename = "../calculated_outputs/terms_" + std::to_string(total_terms) +
                           "_threads_" + std::to_string(num_threads) + "_pi__output.txt";
    
    // create the output file if not exists
    std::ofstream dir_check("../calculated_outputs/.dir_check");
    if (!dir_check.is_open()) {
        system("mkdir -p ../calculated_outputs");
    }

    std::ofstream out(filename);
    if (out.is_open()) {
        // Calculate the number of accurate digits
        unsigned long accurate_digits = total_terms * 14;

        // Set the precision for the output stream
        out.precision(accurate_digits);
        out << std::fixed << pi << std::endl;
        out.close();

        std::cout << "π written to " << filename << " using " << total_terms
                  << " terms, truncated to " << accurate_digits << " digits.\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    // validate 
    if (verify) long long index = validate_pi_files(filename, "../pi_refs/pi-billion.txt");

    // Output the time taken
    std::cout << "Time taken to calculate π: " << duration.count() << " milliseconds.\n";

    return 0;
}
