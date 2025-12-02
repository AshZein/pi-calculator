#include <cuda.h>

__device__ int C = 426880;
__device__ int L = 13591409;
__device__ int X = 640320;
// X3 will be too big for an int
__device__ long long X3 = X * X * X;

