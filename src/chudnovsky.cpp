#include <gmpxx.h>
#include "chudnovsky.h"
// Set working precision (in bits)
#define PRECISION_BITS 100000 // Precision for GMP calculations

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
    mpf_class sum(0);
    mpf_class t(0);

    for (unsigned long k = 0; k < terms; k++) {
        chudnovsky_term(t, k);
        sum += t;
    }

    mpf_class sqrt10005;
    mpf_sqrt_ui(sqrt10005.get_mpf_t(), 10005);

    mpf_class constant = 426880 * sqrt10005;

    pi = constant / sum;
}