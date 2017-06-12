#ifndef includeGuard_1578e50c67e341ccbde0e2dd90aa2159
#define includeGuard_1578e50c67e341ccbde0e2dd90aa2159

const int MAX_GAUSS_R = 3;

const float
GaussCoeff_0[1]={
	1.0/1*1
},
GaussCoeff_1[3]={
	1.0/4*1,
	1.0/42,
	1.0/41
},
GaussCoeff_2[5]={
	1.0/16*1,
	1.0/16*4,
	1.0/16*6,
	1.0/16*4,
	1.0/16*1
},
GaussCoeff_3[7]={
	1.0/64*1,
	1.0/64*6,
	1.0/64*15,
	1.0/64*20,
	1.0/64*15,
	1.0/64*6,
	1.0/64*1
};

const float *GaussCoeff[4] = {
	GaussCoeff_0,
	GaussCoeff_1,
	GaussCoeff_2,
	GaussCoeff_3
};

#endif
