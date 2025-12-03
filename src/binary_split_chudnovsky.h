#include <gmpxx.h>

struct PQT {
    mpz_class P, Q, T;
};

PQT computePQT(unsigned long a, unsigned long b);
void binarySplitPi(mpf_class& pi, unsigned long terms);