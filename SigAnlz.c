#include <stdio.h>
#include <math.h>

#include "SigAnlz.h"
#include "GaussCoef.h"

static int constrain(int n, int min, int max){
	if(n<min)
		return min;
	if(n>max)
		return max;
	return n;
}

void findBase_SigAnlz(float* sig, int n, BitVec *bvptr, float k, int niter){
	float imean, irms, istddev;

	for(int j=0; j<niter; j++){
		param_SigAnlz(sig, n, bvptr, &imean, &irms, &istddev);
		printf("%f, %f, %f\n", imean, irms, istddev);

		for(int i=0; i<n; i++)
			if( sig[i]>imean+k*istddev || sig[i]<imean-k*istddev)
				setBit_BitVec(bvptr, i, 1);
	}
}

void param_SigAnlz(float *sig, int n, BitVec *bvptr, float *mean, float *rms, float *stddev){
	float tmean=0, trms=0, tstddev=0;
	int nSet=0;

	for(int i=0; i<n; i++){
		if(getBit_BitVec(bvptr,i)==0){
			tmean += sig[i];
			trms += sig[i]*sig[i];
			nSet++;
		}
	}

	tmean /= nSet;
	trms = sqrtf(trms/nSet);

	for(int i=0; i<n; i++){
		if(getBit_BitVec(bvptr,i)==0){
			tstddev += (sig[i]-tmean)*(sig[i]-tmean);
		}
	}

	tstddev = sqrtf(tstddev/nSet);

	*mean = tmean;
	*rms = trms;
	*stddev = tstddev;
}



void boxBlur_SigAnlz(float* ip, float *op, int n, int rad){
	float val;
	for(int i=0; i<n; i++){
		val = 0;
		for(int j=i-rad; j<=i+rad; j++){
			val += ip[constrain(j,0,n-1)];
		}
		op[i] = val/(2*rad+1);
	}
}

void gausBlur_SigAnlz(float* ip, float *op, int n, int rad){
	float val;
	for(int i=0; i<n; i++){
		op[i] = 0;
		for(int j=i-rad; j<=i+rad; j++){
			op[i] += ip[constrain(j,0,n-1)]*GaussCoeff[rad][j-i+rad];
		}
	}
}
