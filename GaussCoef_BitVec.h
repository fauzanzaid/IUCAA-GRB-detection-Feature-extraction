#ifndef includeGuard_17dc0c9cc2944fae8020351314888595
#define includeGuard_17dc0c9cc2944fae8020351314888595

const int MAX_GAUSS_R_BitVec = 5;

const float
GaussCoef_0_BitVec[1]={
	1.0/1*1
},
GaussCoef_1_BitVec[3]={
	1.0/4*1,
	1.0/4*2,
	1.0/4*1
},
GaussCoef_2_BitVec[5]={
	1.0/16*1,
	1.0/16*4,
	1.0/16*6,
	1.0/16*4,
	1.0/16*1
},
GaussCoef_3_BitVec[7]={
	1.0/64*1,
	1.0/64*6,
	1.0/64*15,
	1.0/64*20,
	1.0/64*15,
	1.0/64*6,
	1.0/64*1
},
GaussCoef_4_BitVec[9]={
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
GaussCoef_5_BitVec[11]={
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

const float *GaussCoef_BitVec[6] = {
	GaussCoef_0_BitVec,
	GaussCoef_1_BitVec,
	GaussCoef_2_BitVec,
	GaussCoef_3_BitVec,
	GaussCoef_4_BitVec,
	GaussCoef_5_BitVec
};

#endif
