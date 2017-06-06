#ifndef includeGuard_2e23abd6ab6244ef90ce39da8c605365
#define includeGuard_2e23abd6ab6244ef90ce39da8c605365

void readDWTFile_Anlz(float **op, int *n, char *file);
void writeDWTFile_Anlz(float *ip, int n, char *file);

void sort_Anlz(float *ip, float *op, int n);
void sortIdx_Anlz(float *dwt, int **idxSorted, int n);

float sum_Anlz(float *dwt, int n, float *sum);
float max_Anlz(float *dwt, int n, float *max);
float min_Anlz(float *dwt, int n, float *min);
float mean_Anlz(float *dwt, int n, float *mean);
float var_Anlz(float *dwt, int n, float *var);
float stddev_Anlz(float *dwt, int n, float *stddev);
void param_Anlz(float *dwt, int n, float *mean, float *stddev, float *max, float *min);

void denoise_donohoHard_Anlz(float *ip, float *op, int n);
void denoise_donohoSoft_Anlz(float *ip, float *op, int n);
void denoise_Anlz(float *ip, float *op, int n, float k);
void sigCoef_Anlz(float *dwt, int n, float k, int *nsig, int **idxsig);

int jkFromIdx_Anlz(int idx, int *j, int *k);
int jtFromIdx_Anlz(int idx, int n, int *j, int *t, int *tmin, int *tmax);

void readFileAnlzPrint_Anlz(char *file, float k);
#endif
