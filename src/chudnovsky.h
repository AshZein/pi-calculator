#ifndef CHUDNOVSKY_H
#define CHUDNOVSKY_H

#include <gmpxx.h>

#define PRECISION_BITS 100000 // Precision for GMP calculations
// Chudnovsky constants
extern mpz_class A;
extern mpz_class B;
extern mpz_class C;
extern mpz_class D;
extern mpz_class E;


void chudnovsky_term(mpf_class &term, unsigned long k);

#endif // CHUDNOVSKY_H