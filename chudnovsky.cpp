#include <gmpxx.h>

// Set working precision (in bits)
#define PRECISION_BITS 100000 // Precision for GMP calculations

// Chudnovsky constants
const mpz_class C = 426880;
const mpz_class L = 13591409;
const mpz_class X = 640320;
const mpz_class X3 = X * X * X;

// Computes a single Chudnovsky term at index k: returns it in `term`
void chudnovsky_term(mpf_class &term, unsigned long k) {
    // Constants used in the Chudnovsky algorithm
    static const mpz_class C = 640320;
    static const mpz_class C3 = C * C * C;

    // Compute factorials
    mpz_class k_fact, three_k_fact, six_k_fact;
    mpz_fac_ui(k_fact.get_mpz_t(), k);
    mpz_fac_ui(three_k_fact.get_mpz_t(), 3 * k);
    mpz_fac_ui(six_k_fact.get_mpz_t(), 6 * k);

    // Numerator: (-1)^k * (6k)! * (545140134k + 13591409)
    mpz_class num = six_k_fact * (545140134 * k + 13591409);
    if (k % 2 != 0) num = -num;

    // Denominator: (3k)! * (k!)^3 * (640320^3)^k
    mpz_class denom = three_k_fact * k_fact * k_fact * k_fact;
    mpz_class C3_k;
    mpz_pow_ui(C3_k.get_mpz_t(), C3.get_mpz_t(), k);
    denom *= C3_k;

    // Convert to floating point with correct precision
    mpf_class f_num(num, PRECISION_BITS);
    mpf_class f_denom(denom, PRECISION_BITS);

    // Compute term
    term.set_prec(PRECISION_BITS);
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
