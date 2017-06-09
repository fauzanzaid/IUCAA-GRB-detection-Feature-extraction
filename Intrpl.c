#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Intrpl.h"

int linear_Intrpl(float *ip, int ipsz, float *op, int opsz){
	
	float i;
	int j;

	int iI;
	float iIF, iF;

	op[0] = ip[0];	// Not in loop to avoid possible fp errors at the ends
	for(j=1; j<opsz-1; j++){
		i = ((float)j)*(ipsz-1)/(opsz-1);
		iF = modff(i, &iIF);
		iI = (int)iIF;
		op[j] = (1-iF)*ip[iI] + iF*ip[iI+1];
	}
	op[opsz-1] = ip[ipsz-1];	// Not in loop to avoid possible fp errors at the ends

	return 0;
}

int cubic_Intrpl(float *ip, int ipsz, float *op, int opsz){
	
	return 0;
}
