#include <gmpxx.h>
#include "chudnovsky.h"
// Set working precision (in bits)
#define PRECISION_BITS 100000 // Precision for GMP calculations

// Chudnovsky constants
// const mpz_class C = 426880;
// const mpz_class L = 13591409;
// const mpz_class X = 640320;
// const mpz_class X3 = X * X * X;

void chudnovsky_term(mpf_class &term, unsigned long k) {
    const mpz_class A = 13591409;
    const mpz_class B = 545140134;
    const mpz_class C = 640320;
    const mpz_class C3_OVER_24 = (C * C * C) / 24;

    mpz_class numerator;
    mpz_class factorial_6k;
    mpz_fac_ui(factorial_6k.get_mpz_t(), 6 * k);
    numerator = factorial_6k * (A + B * k);

    mpz_class factorial_3k, factorial_k;
    mpz_fac_ui(factorial_3k.get_mpz_t(), 3 * k);
    mpz_fac_ui(factorial_k.get_mpz_t(), k);
    mpz_class denominator = factorial_3k * factorial_k * factorial_k * factorial_k;
    mpz_class power_C3_OVER_24;
    mpz_pow_ui(power_C3_OVER_24.get_mpz_t(), C3_OVER_24.get_mpz_t(), k);
    denominator *= power_C3_OVER_24;

    term = mpf_class(numerator) / mpf_class(denominator);

    if (k % 2 != 0) {
        term = -term;
    }
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
