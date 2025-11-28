// filepath: /home/ash/Desktop/Projects/pi-calculator/chudnovsky.h
#ifndef CHUDNOVSKY_H
#define CHUDNOVSKY_H

#include <gmpxx.h>

#define PRECISION_BITS 100000 // Precision for GMP calculations
// Chudnovsky constants
const mpz_class C = 426880;
const mpz_class L = 13591409;
const mpz_class X = 640320;
const mpz_class X3 = X * X * X;

void chudnovsky_term(mpf_class &term, unsigned long k);

#endif // CHUDNOVSKY_H