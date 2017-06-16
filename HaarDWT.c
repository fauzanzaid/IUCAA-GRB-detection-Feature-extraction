#include "HaarDWT.h"
#include <stdio.h>
#include <string.h>


const float sqrthalf = 0.70710678118;

static void rec_coef_1D_HaarDWT(float *ip, float *op, long n);




void rec_coef_1D_HaarDWT(float *ip, float *op, long n){
	if(n==1)
		return;	// Base case

	int i = 0;

	for ( ; i<n/2; ++i)	// approx, scale, lo freq
		op[i] = ip[2*i] + ip[2*i+1];
	for ( ; i<n; ++i)	// detail, wavelet, hi freq
		op[i] = ip[2*i-n] - ip[2*i-n+1];
	for ( i=0 ; i<n; ++i)	// normailze
		op[i] *= sqrthalf;

	// for ( i=0 ; i<n; ++i)
	// 	printf("%f\t", op[i]);
	// printf("\n");

	float ip2[n/2];
	for ( i=0 ; i<n/2; ++i)
		ip2[i] = op[i];

	rec_coef_1D_HaarDWT(ip2, op, n/2);
}




void coef_1D_HaarDWT(float *ip, float *op, long n){
	if( (n==0) || (n&(n-1)) ){	// Check if n is not a power of 2
		printf("%ld is not a power of 2\n", n);
		return;
	}


	rec_coef_1D_HaarDWT(ip, op, n);

}


void coef_1D_if_HaarDWT(int *ip, float *op, long n){

	if( (n==0) || (n&(n-1)) ){	// Check if n is not a power of 2
		printf("%ld  not a power of 2\n", n);
		return;
	}

	if(n==1){
		op[0] = ip[0];
		return;
	}

	int tp[n];
	memcpy(tp, ip, sizeof(tp));	// Copy array ip to tp

	for(int k=1; k<n; k*=2){	// k=2^j, j is scale
		for(int i=0; i<n; i+=2*k){
			tp[i] = tp[i] + tp[i+k];
			tp[i+k] = tp[i] - 2*tp[i+k];
		}
	}

	int j = n-1;
	for(int k=1; k<n; k*=2){
		for(int i=k; i<n; i+=2*k, j--){
			op[j] = tp[i];
		}
	}
	op[0] = tp[0];

	for(int k=2; k<=n; k*=2){
		for(int i=0; i<k; ++i)
			op[i] *= sqrthalf;
	}
}


void sig_1D_HaarDWT(float *ip, float *op, long n){
	return;
}
