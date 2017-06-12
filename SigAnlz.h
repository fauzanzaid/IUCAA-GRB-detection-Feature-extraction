#ifndef pa7d3025da6e4da7a37a65dde22b7072
#define pa7d3025da6e4da7a37a65dde22b7072

#include "BitVec.h"

void findBase_SigAnlz(float* sig, int n, BitVec *bvptr, float k, int niter);
void param_SigAnlz(float *sig, int n, BitVec *bvptr, float *mean, float *rms, float *stddev);

#endif