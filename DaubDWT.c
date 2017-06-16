#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DaubDWT.h"
#include "DaubCoef.h"



void coef_1D_Dx_DaubDWT(float *ip, float *op, long n, int x){

	if(x>MAX_X || x<2 || x%2!=0){
		printf("%d not an even integer between 2 and %d\n", x, MAX_X);
		return;
	}

	if( (n==0) || (n&(n-1)) ){	// Check if n is not a power of 2
		printf("%ld not a power of 2\n", n);
		return;
	}

	if(n==1){
		op[0] = ip[0];
		return;
	}

	float *tpip = malloc( (n)*sizeof(float) );
	memcpy(tpip, ip, n*sizeof(float) );	// Copy array ip to tp
	float *tpop = malloc( (n)*sizeof(float) );

	for(int k=n; k>1; k/=2){

		for(int i=0; i<k/2; i++){

			tpop[i] = tpop[i+k/2] = 0;	// Clear for incremental sum
			for(int j=0; j<x; ++j){
				tpop[i] += tpip[(2*i+j)%k]*Daub_DX_LD[x/2-1][j];	// Scale. % for wraparound
				tpop[i+k/2] += tpip[(2*i+j)%k]*Daub_DX_HD[x/2-1][j];	// Wavelet
			}
		}

		memcpy(tpip, tpop, k*sizeof(float) );	// Copy for next iteration

	}

	for(int i=0; i<n; i++)
		op[i] = tpop[i];

	free(tpip);
	free(tpop);
}

void print_coef_DaubDWT(){
	for(int i=0; i<4; i++){
		for(int j=0; j<(i+1)*2; j++){
			printf("%f\t@ %p\t%f\t@ %p\t%d:%d\n", Daub_DX_LD[i][j], &(Daub_DX_LD[i][j]), Daub_DX_HD[i][j], &(Daub_DX_HD[i][j]), i, j);
		}
		printf("\n");
	}
}