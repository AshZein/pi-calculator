#include <gmpxx.h>s
#include <cuda.h>

int chooseBlockSize(int warpSize, int maxThreadsPerBlock) {
    int sizes[] = {1024, 512, 256, 128, 64};
    for (int s : sizes)
        if (s <= maxThreadsPerBlock && (s % warpSize == 0))
            return s;
    return warpSize;  // fallback
}

void calculate_pi_gpu(mpf_class& pi, unsigned long terms){
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    int blocksize = chooseBlockSize(prop.warpsize, prop.maxThreadsPerBlock);

    
}