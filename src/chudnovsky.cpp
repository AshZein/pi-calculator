#include <gmpxx.h>
#include "chudnovsky.h"
// Set working precision (in bits)
#define PRECISION_BITS 100000 // Precision for GMP calculations

// Chudnovsky constants
// const mpz_class C = 426880;
// const mpz_class L = 13591409;
// const mpz_class X = 640320;
// const mpz_class X3 = X * X * X;

void chudnovsky_term(mpf_class& term, unsigned long k) {
    const mpz_class A = 13591409;
    const mpz_class B = 545140134;

    mpz_class num, denom;
    
    mpz_class f6k, f3k, fk;
    mpz_fac_ui(f6k.get_mpz_t(), 6*k);
    mpz_fac_ui(f3k.get_mpz_t(), 3*k);
    mpz_fac_ui(fk .get_mpz_t(), k);

    num = f6k * (A + B*k);

    denom = f3k * fk * fk * fk;

    mpz_class p;
    mpz_pow_ui(p.get_mpz_t(), mpz_class(640320).get_mpz_t(), 3*k);
    denom *= p;

    term = mpf_class(num) / mpf_class(denom);

    if (k & 1) term = -term;
}

void calculate_pi(mpf_class& pi, unsigned long terms) {

    mpf_set_default_prec(PRECISION_BITS);

    mpf_class sum = 0;
    mpf_class t;

    for (unsigned long k = 0; k < terms; k++) {
        chudnovsky_term(t, k);
        sum += t;
    }

    mpf_class sqrt10005;
    mpf_sqrt_ui(sqrt10005.get_mpf_t(), 10005);

    mpf_class constant = 426880 * sqrt10005;

    pi = constant / sum;
}


// void chudnovsky_term(mpf_class &term, unsigned long k) {
//     const mpz_class A = 13591409;
//     const mpz_class B = 545140134;
//     const mpz_class C = 640320;
//     const mpz_class C3_OVER_24 = (C * C * C) / 24;

//     mpz_class numerator;
//     mpz_class factorial_6k;
//     mpz_fac_ui(factorial_6k.get_mpz_t(), 6 * k);
//     numerator = factorial_6k * (A + B * k);

//     mpz_class factorial_3k, factorial_k;
//     mpz_fac_ui(factorial_3k.get_mpz_t(), 3 * k);
//     mpz_fac_ui(factorial_k.get_mpz_t(), k);
//     mpz_class denominator = factorial_3k * factorial_k * factorial_k * factorial_k;
//     mpz_class power_C3_OVER_24;
//     mpz_pow_ui(power_C3_OVER_24.get_mpz_t(), C3_OVER_24.get_mpz_t(), k);
//     denominator *= power_C3_OVER_24;

//     mpz_class C_pow_3k_3_2;
//     mpz_pow_ui(C_pow_3k_3_2.get_mpz_t(), C.get_mpz_t(), 3 * k +(3 / 2));
//     denominator = factorial_3k * factorial_k * factorial_k * factorial_k * C_pow_3k_3_2;

//     term = mpf_class(numerator) / mpf_class(denominator);

//     if (k % 2 != 0) {
//         term = -term;
//     }
// }

// // Calculates pi using the Chudnovsky algorithm with `terms` number of terms
// void calculate_pi(mpf_class &pi, unsigned long terms) {
//     mpf_set_default_prec(PRECISION_BITS);

//     mpf_class sum = 0.0;
//     mpf_class term;

//     for (unsigned long k = 0; k < terms; ++k) {
//         chudnovsky_term(term, k);
//         sum += term;
//     }

//     mpz_class rightside = 12 * sum;
//     pi = 1 / rightside;

//     return pi;
//     // Final multiplication: pi = C * sqrt(10005) / sum
//     // mpf_class sqrt_val;
//     // mpf_sqrt_ui(sqrt_val.get_mpf_t(), 10005);
//     // sqrt_val *= C;

//     // pi = sqrt_val / sum;
// }
