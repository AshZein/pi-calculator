#include <pthread.h>
#include <iostream>
#include <vector>
#include <gmpxx.h>
#include <cmath>
#include <fstream>
#include <chrono> // Include for timing

#include "chudnovsky.h"

int NUM_THREADS = 2; // Number of threads to use
std::vector<mpf_class> term_results; // Shared array for storing term results
pthread_mutex_t term_mutex; // Mutex for thread-safe access to shared resources

unsigned long total_terms; // Total number of terms to compute
// #define PRECISION_BITS 100000 // Precision for GMP calculations

// // Chudnovsky constants
// const mpz_class C = 426880;

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

void threaded_calculate_pi(mpf_class &pi, unsigned long terms, int threads){
    mpf_set_default_prec(PRECISION_BITS);

    total_terms = terms;
    if (total_terms == 0) {
        std::cerr << "Invalid number of terms: " << terms << "\n";
        return;
    }

    if (threads > 0) {
        NUM_THREADS = threads;
        std::cout << "Using " << NUM_THREADS << " threads.\n";
    } else {
        std::cout << "No thread count provided. Using default: " << NUM_THREADS << " threads.\n";
    }

    pthread_mutex_init(&term_mutex, nullptr);
    term_results.resize(total_terms, 0.0); // Initialize the shared array

    mpf_class sum = 0.0; // Global sum

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();

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

    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    mpf_class sqrt_val;
    mpf_sqrt_ui(sqrt_val.get_mpf_t(), 10005);
    sqrt_val *= C;

    pi = sqrt_val / sum;

    // std::ofstream out("calculated_outputs/pi_threaded_output.txt");
    // if (out.is_open()) {
}