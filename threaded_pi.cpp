#include <pthread.h>
#include <iostream>
#include <vector>
#include <gmpxx.h>  // For GMP arbitrary precision
#include <cmath>    // For mathematical functions
#include <fstream>

int NUM_THREADS = 4; // Number of threads to use
mpf_class sum = 0.0; // Global sum variable
pthread_mutex_t sum_mutex; // Mutex for thread-safe access to sum
pthread_cond_t cond_var; // Condition variable for thread synchronization
pthread_barrier_t barrier; // Barrier for thread synchronization

unsigned long total_terms; // Total number of terms to compute. 10 is default

// Chudnovsky constants
const mpz_class C = 426880;

void chudnovsky_term(mpf_class &term, unsigned long k);

int start_work(){
    mpf_class term;
    mpf_class thread_sum = 0.0;

    // Get the thread's ID
    long thread_id = (long)pthread_self();
    std::cout << "Thread ID: " << thread_id << std::endl;

    unsigned long start_term = thread_id * (total_terms / NUM_THREADS);
    unsigned long end_term = (thread_id + 1) * (total_terms / NUM_THREADS);
    
    if (thread_id == NUM_THREADS - 1) {
        end_term = total_terms; // Last thread takes the remainder
    } 

    for (unsigned long k = start_term; k < end_term; k++){
        // Compute the k-th term using the Chudnovsky algorithm
        chudnovsky_term(term, k);
        thread_sum += term;
    }

    // Lock the mutex before updating the global sum
    pthread_mutex_lock(&sum_mutex);
    sum += thread_sum; // Update the global sum
    pthread_mutex_unlock(&sum_mutex); // Unlock the mutex

    pthread_barrier_wait(&barrier); // Wait for all threads to finish

    if (thread_id == 0) {
        // Notify all threads that the sum is ready
        pthread_cond_broadcast(&cond_var);
    } else {
        // Wait for the sum to be ready
        pthread_cond_wait(&cond_var, &sum_mutex);
    }
}

int main(int argc, char* argv[]) {
    const unsigned long PRECISION_BITS = 100000;
    const unsigned long DEFAULT_TERMS = 10;

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
    pthread_mutex_init(&sum_mutex, nullptr); // Initialize the mutex
    pthread_cond_init(&cond_var, nullptr); // Initialize the condition variable
    pthread_barrier_init(&barrier, nullptr, NUM_THREADS); // Initialize the barrier
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], nullptr, start_work, (void*)i); // Create threads
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr); // Wait for threads to finish
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
        std::cout << "π written to pi_threaded_output.txt using " << terms << " terms\n";
    } else {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    return 0;
}
