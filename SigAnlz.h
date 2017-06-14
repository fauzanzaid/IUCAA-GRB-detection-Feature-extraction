#ifndef includeGuard_pa7d3025da6e4da7a37a65dde22b7072
#define includeGuard_pa7d3025da6e4da7a37a65dde22b7072

#include "BitVec.h"

void threshold_SigAnlz(float* sig, int n, BitVec *bvptr, float k, float diff);
void param_SigAnlz(float *sig, int n, BitVec *bvptr, float *mean, float *rms, float *stddev);

void boxBlur_SigAnlz(float* ip, float *op, int n, int rad);
void gaussBlur_SigAnlz(float* ip, float *op, int n, int rad);

void ignoreDipBitVec_SigAnlz(BitVec* ip, BitVec *op, float *sig, int n, float k);

#endif
