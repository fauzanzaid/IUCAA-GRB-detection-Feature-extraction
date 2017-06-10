#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Intrpl.h"

int linear_Intrpl(float *ip, int ipsz, float *op, int opsz){
	
	int j;
	float map;

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

int cubic_Intrpl(float *ip, int ipsz, float *op, int opsz){	// Cubic Hermite
	
	int j;
	float map;

	int i;
	float iI, iF;

	op[0] = ip[0];
	for(j=1; j<opsz-1; j++){
		map = ((float)j)*(ipsz-1)/(opsz-1);
		iF = modff(map, &iI);
		i = (int)iI;

		if(i==0 || i==ipsz-2)	// Avoid index out of bound, switch to linear
			op[j] = (1-iF)*ip[i] + iF*ip[i+1];
		else
			op[j] = 0.5*(
				ip[i-1]	*(-iF*iF*iF + 2*iF*iF -iF) +
				ip[i]	*(3*iF*iF*iF - 5*iF*iF + 2) +
				ip[i+1]	*(-3*iF*iF*iF + 4*iF*iF + iF) +
				ip[i+2]	*(iF*iF*iF - iF*iF)
				);

	}
	op[opsz-1] = ip[ipsz-1];

	return 0;
}
