#ifndef includeGuard_pa7d3025da6e4da7a37a65dde22b7072
#define includeGuard_pa7d3025da6e4da7a37a65dde22b7072

#include "BitVec.h"

void findBase_SigAnlz(float* sig, int n, BitVec *bvptr, float k, int niter);
void param_SigAnlz(float *sig, int n, BitVec *bvptr, float *mean, float *rms, float *stddev);

void boxBlur_SigAnlz(float* ip, float *op, int n, int rad);
void gaussBlur_SigAnlz(float* ip, float *op, int n, int rad);

#endif