#include <pthread.h>
#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <cmath>
#include <fstream>

#include "chudnovsky.h"

int NUM_THREADS = 4; // Number of threads to use
std::vector<mpf_class> term_results; // Shared array for storing term results
pthread_mutex_t term_mutex; // Mutex for thread-safe access to shared resources

unsigned long total_terms; // Total number of terms to compute
#define PRECISION_BITS 100000 // Precision for GMP calculations
#define DEFAULT_TERMS 10

// Chudnovsky constants
const mpz_class C = 426880;

// Thread function to calculate a single term
void* calculate_term(void* arg) {
    unsigned long term_index = *(unsigned long*)arg;
    delete (unsigned long*)arg;

    if (term_index >= total_terms) {
        return nullptr; // Out of bounds, do nothing
    }

    mpf_class term;
    chudnovsky_term(term, term_index);

    // Store the result in the shared array
    pthread_mutex_lock(&term_mutex);
    term_results[term_index] = term;
    pthread_mutex_unlock(&term_mutex);

    return nullptr;
}

int main(int argc, char* argv[]) {
    mpf_set_default_prec(PRECISION_BITS);

    total_terms = DEFAULT_TERMS;
    if (argc >= 2) {
        total_terms = std::strtoul(argv[1], nullptr, 10);
        if (total_terms == 0) {
            std::cerr << "Invalid number of terms: " << argv[1] << "\n";
            return 1;
        }
    } else {
        std::cout << "No term count provided. Using default: " << DEFAULT_TERMS << " terms.\n";
    }

    pthread_mutex_init(&term_mutex, nullptr);
    term_results.resize(total_terms, 0.0); // Initialize the shared array

    mpf_class sum = 0.0; // Global sum

    unsigned long current_term = 0;
    while (current_term < total_terms) {
        pthread_t threads[NUM_THREADS];
        unsigned long terms_to_calculate = std::min(static_cast<unsigned long>(NUM_THREADS), total_terms - current_term);

        // Spawn threads to calculate the next batch of terms
        for (unsigned long i = 0; i < terms_to_calculate; ++i) {
            unsigned long* term_index = new unsigned long(current_term + i);
            pthread_create(&threads[i], nullptr, calculate_term, term_index);
        }

        // Wait for all threads in the batch to finish
        for (unsigned long i = 0; i < terms_to_calculate; ++i) {
            pthread_join(threads[i], nullptr);
        }

        // Perform reduction for the current batch
        for (unsigned long i = 0; i < terms_to_calculate; ++i) {
            sum += term_results[current_term + i];
        }

        current_term += terms_to_calculate; // Move to the next batch
    }

    mpf_class sqrt_val;
    mpf_sqrt_ui(sqrt_val.get_mpf_t(), 10005);
    sqrt_val *= C;

    mpf_class pi = sqrt_val / sum;

    std::ofstream out("calculated_outputs/pi_threaded_output.txt");
    if (out.is_open()) {
        // Calculate the number of accurate digits
        unsigned long accurate_digits = total_terms * 14;

        // Set the precision for the output stream
        out.precision(accurate_digits);
        out << std::fixed << pi << std::endl;
        out.close();

        std::cout << "π written to pi_threaded_output.txt using " << total_terms
                  << " terms, truncated to " << accurate_digits << " digits.\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    pthread_mutex_destroy(&term_mutex);

    return 0;
}
