#include <gmpxx.h>

// Set working precision (in bits)
const unsigned long PRECISION_BITS = 10000; // ~3000 decimal digits

// Chudnovsky constants
const mpz_class C = 426880;
const mpz_class L = 13591409;
const mpz_class X = 640320;
const mpz_class X3 = X * X * X;

// Computes a single Chudnovsky term at index k: returns it in `term`
void chudnovsky_term(mpf_class &term, unsigned long k) {
    mpf_set_default_prec(PRECISION_BITS);

    mpz_class six_k_fact = 1;
    mpz_class three_k_fact = 1;
    mpz_class k_fact = 1;
    mpz_class num, denom;
    mpz_class x_pow;

    // Compute k!, (3k)!, (6k)!
    for (unsigned long i = 1; i <= k; ++i)
        k_fact *= i;
    for (unsigned long i = 1; i <= 3 * k; ++i)
        three_k_fact *= i;
    for (unsigned long i = 1; i <= 6 * k; ++i)
        six_k_fact *= i;

    // Compute (-1)^k
    int sign = (k % 2 == 0) ? 1 : -1;

    // x^3k
    x_pow = 1;
    for (unsigned long i = 0; i < k; ++i)
        x_pow *= X3;

    // Numerator
    num = sign * six_k_fact * (L + 545140134 * k);

    // Denominator
    mpz_class k_fact_cubed = k_fact * k_fact * k_fact;
    denom = three_k_fact * k_fact_cubed * x_pow;

    // Convert to mpf_class
    mpf_class f_num(num);
    mpf_class f_denom(denom);

    term = f_num / f_denom;
}

// Calculates pi using the Chudnovsky algorithm with `terms` number of terms
void calculate_pi(mpf_class &pi, unsigned long terms) {
    mpf_set_default_prec(PRECISION_BITS);

    mpf_class sum = 0.0;
    mpf_class term;

    for (unsigned long k = 0; k < terms; ++k) {
        chudnovsky_term(term, k);
        sum += term;
    }

    // Final multiplication: pi = C * sqrt(10005) / sum
    mpf_class sqrt_val;
    mpf_sqrt_ui(sqrt_val.get_mpf_t(), 10005);
    sqrt_val *= C;

    pi = sqrt_val / sum;
}
