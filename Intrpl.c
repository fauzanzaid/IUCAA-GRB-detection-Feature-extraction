#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Intrpl.h"

int linear_Intrpl(float *ip, int ipsz, float *op, int opsz){
	
	float map;
	int j;

	int i;
	float iI, iF;

	op[0] = ip[0];	// Not in loop to avoid possible fp errors
	for(j=1; j<opsz-1; j++){
		map = ((float)j)*(ipsz-1)/(opsz-1);
		iF = modff(map, &iI);
		i = (int)iI;
		op[j] = (1-iF)*ip[i] + iF*ip[i+1];
	}
	op[opsz-1] = ip[ipsz-1];	// Not in loop to avoid possible array out of bound

	return 0;
}

int cubic_Intrpl(float *ip, int ipsz, float *op, int opsz){
	
	return 0;
}
