#ifndef includeGuard_1578e50c67e341ccbde0e2dd90aa2159
#define includeGuard_1578e50c67e341ccbde0e2dd90aa2159

const int MAX_GAUSS_R = 5;

const float
GaussCoeff_0[1]={
	1.0/1*1
},
GaussCoeff_1[3]={
	1.0/4*1,
	1.0/4*2,
	1.0/4*1
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
},
GaussCoeff_4[9]={
	1.0/256*1,
	1.0/256*8,
	1.0/256*28,
	1.0/256*56,
	1.0/256*70,
	1.0/256*56,
	1.0/256*28,
	1.0/256*8,
	1.0/256*1
},
GaussCoeff_5[11]={
	1.0/1024*1,
	1.0/1024*10,
	1.0/1024*45,
	1.0/1024*120,
	1.0/1024*210,
	1.0/1024*252,
	1.0/1024*210,
	1.0/1024*120,
	1.0/1024*45,
	1.0/1024*10,
	1.0/1024*1
};

const float *GaussCoeff[6] = {
	GaussCoeff_0,
	GaussCoeff_1,
	GaussCoeff_2,
	GaussCoeff_3,
	GaussCoeff_4,
	GaussCoeff_5
};

#endif
