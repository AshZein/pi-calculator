#include <gmp.h>
#include <gmpxx.h>

// Function to calculate factorial
void factorial(mpz_class &result, unsigned long n) {
    mpz_set_ui(result.get_mpz_t(), 1);
    for (unsigned long i = 1; i <= n; ++i) {
        mpz_mul_ui(result.get_mpz_t(), result.get_mpz_t(), i);
    }
}

// Function to calculate the nth term of the Chudnovsky series
void chudnovsky_term(mpz_class &numerator, mpz_class &denominator, unsigned long n) {
    mpz_class factorial_6n, factorial_3n, factorial_n;
    factorial(factorial_6n, 6 * n);
    factorial(factorial_3n, 3 * n);
    factorial(factorial_n, n);

    mpz_class C = 545140134;
    mpz_class L = 13591409 + C * n;

    // Calculate X = (-262537412640768000)^n
    mpz_class X;
    mpz_pow_ui(X.get_mpz_t(), mpz_class(-262537412640768000).get_mpz_t(), n);

    // Calculate numerator and denominator
    numerator = factorial_6n * L;

    mpz_class factorial_n_cubed;
    mpz_pow_ui(factorial_n_cubed.get_mpz_t(), factorial_n.get_mpz_t(), 3); // factorial_n^3

    denominator = factorial_3n * factorial_n_cubed * X;
}

// Function to calculate pi using Chudnovsky's algorithm
void calculate_pi(mpf_class &pi, unsigned long precision) {
    mpf_set_default_prec(precision * 3.32193); // Set precision in bits
    mpf_class sum(0, precision);
    mpf_class term(0, precision);

    mpz_class numerator, denominator;
    for (unsigned long n = 0; n < precision; ++n) {
        chudnovsky_term(numerator, denominator, n);
        term = mpf_class(numerator) / mpf_class(denominator);
        sum += term;

        if (term == 0) {
            break; // Stop if the term becomes zero
        }
    }

    mpf_class constant(426880 * sqrt(10005), precision);
    pi = constant / sum;
}