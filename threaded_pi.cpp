#include <pthread.h>
#include <iostream>
#include <vector>
#include <gmpxx.h>  // For GMP arbitrary precision
#include <cmath>    // For mathematical functions
#include <fstream>

int NUM_THREADS = 4; // Number of threads to use
mpf_class sum = 0.0; // Global sum variable
int *thread_sums;
pthread_mutex_t sum_mutex; // Mutex for thread-safe access to sum
pthread_cond_t cond_var; // Condition variable for thread synchronization
pthread_barrier_t barrier; // Barrier for thread synchronization

unsigned long total_terms; // Total number of terms to compute. 10 is default
#define PRECISION_BITS 100000 // Precision for GMP calculations
#define DEFAULT_TERMS 10

// Chudnovsky constants
const mpz_class C = 426880;

int NUM_TERMS_THREAD = 10;
std::vector<mpf_class> terms(NUM_THREADS * NUM_TERMS_THREAD, mpf_class(0, PRECISION_BITS)); // Use a vector for dynamic allocation
unsigned long current_term = 0; // Current term index

void chudnovsky_term(mpf_class &term, unsigned long k);

void* start_work(void* arg) {
    // THIS WAY HAS A LOT OF FALSE SHARING
    long thread_id = *(long*)arg; // Cast the argument to a long pointer and dereference
    delete (long*)arg; // Free the dynamically allocated memory

    unsigned long start_term = current_term + thread_id;
    unsigned long end_term = start_term + (NUM_TERMS_THREAD * NUM_THREADS - (NUM_THREADS - thread_id));

    if (end_term > total_terms){
        if (thread_id == NUM_THREADS - 1) {
            end_term = total_terms; // Last thread takes the remainder
        } else {
            end_term = start_term + ((NUM_TERMS_THREAD - 1) * NUM_THREADS - (NUM_THREADS - thread_id));
        }
    }

    for (unsigned long k = start_term; k < end_term; k+= NUM_THREADS) {
        // Compute the k-th term using the Chudnovsky algorithm
        chudnovsky_term(terms[k], k);
    }

    return nullptr; // Return nullptr to satisfy the void* return type
}

int main(int argc, char* argv[]) {
    mpf_set_default_prec(PRECISION_BITS);

    // Get number of terms from argv, or use default
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

    mpf_class pi;

    pthread_t threads[NUM_THREADS]; // Array to hold thread IDs

    thread_sums = new int[NUM_THREADS]; // Array to hold sums from each thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_sums[i] = 0; // Initialize thread sums to zero
    }
    
    pthread_mutex_init(&sum_mutex, nullptr); // Initialize the mutex
    pthread_cond_init(&cond_var, nullptr); // Initialize the condition variable
    pthread_barrier_init(&barrier, nullptr, NUM_THREADS); // Initialize the barrier

    while (current_term < total_terms) {

        for (int i = 0; i < NUM_THREADS; ++i) {
            long* thread_id = new long(i); // Dynamically allocate memory for thread ID
            pthread_create(&threads[i], nullptr, start_work, thread_id); // Create threads
        }

        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(threads[i], nullptr); // Wait for threads to finish
        }

        // Perform addition reduction on the terms array into the global sum
        for (unsigned long i = 0; i < NUM_THREADS * NUM_TERMS_THREAD; i++) {
            sum += terms[i];
            terms[i] = 0; // Reset the term after adding to the sum
        }

        current_term += NUM_THREADS * NUM_TERMS_THREAD; // Update the current term index
        if (current_term >= total_terms) {
            break; // Exit the loop if all terms have been computed
        }
    }
    // Final multiplication: pi = C * sqrt(10005) / sum
    mpf_class sqrt_val;
    mpf_sqrt_ui(sqrt_val.get_mpf_t(), 10005);
    sqrt_val *= C;

    pi = sqrt_val / sum;

    std::ofstream out("pi_threaded_output.txt");
    if (out.is_open()) {
        out.precision(pi.get_prec() * 0.30103); // bits to decimal digits
        out << std::fixed << pi << std::endl;
        out.close();
        std::cout << "π written to pi_threaded_output.txt using " << total_terms << " terms\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    return 0;
}
