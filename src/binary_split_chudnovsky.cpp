#include <gmpxx.h>
#include "binary_split_chudnovsky.h"
#include "chudnovsky.h"

mpz_class C3_24 = C * C * C / 24;

PQT computePQT(unsigned long a, unsigned long b){
    int m;
    PQT res;

    if (a + 1 == b) {
        res.P  = (2 * b - 1);
        res.P *= (6 * b - 1);
        res.P *= (6 * b - 5);
        res.Q  = C3_24 * b * b * b;
        res.T  = (A + B * b) * res.P;
        if ((b & 1) == 1) res.T = - res.T;
    } else {
        m = (a + b) / 2;
        PQT res1 = computePQT(a, m);
        PQT res2 = computePQT(m, b);
        res.P = res1.P * res2.P;
        res.Q = res1.Q * res2.Q;
        res.T = res1.T * res2.Q + res1.P * res2.T;
    }

    return res;
}

void binarySplitPi(mpf_class& pi, unsigned long terms){
    unsigned long numDigits = terms * 14;

    PQT pqt = computePQT(0, numDigits);

    pi = D * sqrt((mpf_class)E) * pqt.Q;
    pi /= (A * pqt.Q + pqt.T);
}