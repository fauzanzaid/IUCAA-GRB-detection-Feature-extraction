#ifndef includeGuard_2e23abd6ab6244ef90ce39da8c605365
#define includeGuard_2e23abd6ab6244ef90ce39da8c605365

void sort_DWTAnlz(float *ip, float *op, int n);
void sortIdx_DWTAnlz(float *dwt, int **idxSorted, int n);

float sum_DWTAnlz(float *dwt, int n, float *sum);
float max_DWTAnlz(float *dwt, int n, float *max);
float min_DWTAnlz(float *dwt, int n, float *min);
float mean_DWTAnlz(float *dwt, int n, float *mean);
float var_DWTAnlz(float *dwt, int n, float *var);
float stddev_DWTAnlz(float *dwt, int n, float *stddev);
void param_DWTAnlz(float *dwt, int n, float *mean, float *stddev, float *max, float *min);

void denoise_donohoHard_DWTAnlz(float *ip, float *op, int n);
void denoise_donohoSoft_DWTAnlz(float *ip, float *op, int n);
void denoise_DWTAnlz(float *ip, float *op, int n, float k);
void sigCoef_DWTAnlz(float *dwt, int n, float k, int *nsig, int **idxsig);
void ratioFixed_DWTAnlz(float* dwt, int n, float *ratio, int nRatio);
void ratioMoving_DWTAnlz(float* dwt, int n, float *ratio, int nRatio);

int jkFromIdx_DWTAnlz(int idx, int *j, int *k);
int jtFromIdx_DWTAnlz(int idx, int n, int *j, int *t, int *tmin, int *tmax);

#endif
