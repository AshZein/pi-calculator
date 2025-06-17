// filepath: /home/ash/Desktop/Projects/pi-calculator/chudnovsky.h
#ifndef CHUDNOVSKY_H
#define CHUDNOVSKY_H

#include <gmpxx.h>

#define PRECISION_BITS 100000 // Precision for GMP calculations
// Chudnovsky constants
const mpz_class C = 426880;

void chudnovsky_term(mpf_class &term, unsigned long k);

#endif // CHUDNOVSKY_H