#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BitVec.h"
#include "GaussCoef_BitVec.h"



static int constrain_ext(int n, int min, int max){
	if(n<min)
		return min;
	if(n>max)
		return max;
	return n;
}



typedef struct BitVec{
	uint8_t *arr;
	int sz;
} BitVec;



BitVec *new_BitVec(int sz){
	BitVec *bvptr = malloc(sizeof(BitVec));

	int arrsz = (sz+7)/8;
	bvptr->arr = malloc(arrsz*sizeof(uint8_t));

	memset(bvptr->arr, 0, arrsz*sizeof(uint8_t));
	bvptr->sz = sz;

	return bvptr;
}

BitVec *duplicate_BitVec(BitVec *bvptr){
	BitVec *dpptr = malloc(sizeof(BitVec));

	int arrsz = (bvptr->sz+7)/8;
	dpptr->arr = malloc(arrsz*sizeof(uint8_t));

	memcpy(dpptr->arr, bvptr->arr, arrsz*sizeof(uint8_t));
	dpptr->sz = bvptr->sz;

	return dpptr;
}

void free_BitVec(BitVec *bvptr){
	free(bvptr->arr);
	free(bvptr);
}



int nSet_BitVec(BitVec *bvptr){
	int arrsz = (bvptr->sz+7)/8;
	int nSet = 0;

	for(int i=0; i<arrsz; i++){
		for(int j=1; j<256; j<<=1){
			if( (bvptr->arr)[i] & j )
				nSet++;
		}
	}
	return nSet;
}

void setBit_BitVec(BitVec *bvptr, int idx, int val){
	int arridx = idx/8;
	int bitidx = idx%8;
	
	if(val==0)
		(bvptr->arr)[arridx] &= ~(1<<bitidx);
	else if(val==1)
		(bvptr->arr)[arridx] |= 1<<bitidx;
}

void setAll_BitVec(BitVec *bvptr, int val){
	int arrsz = (bvptr->sz+7)/8;
	if(val==0)
		memset(bvptr->arr, 0, arrsz*sizeof(uint8_t));
	else if(val==1){
			memset(bvptr->arr, 255, arrsz*sizeof(uint8_t));
			(bvptr->arr)[arrsz-1] &= 255<<(bvptr->sz%8);	// Set bits out of bound to zero
	}
}

int getBit_BitVec(BitVec *bvptr, int idx){
	int val;
	int arridx = idx/8;
	int bitidx = idx%8;
	val = bvptr->arr[arridx] & 1<<bitidx;
	
	if(val==0)
		return 0;
	else
		return 1;
}



int getSize_BitVec(BitVec *bvptr){
	return bvptr->sz;
}


// Check for val at and after iter position
int hasNext_BitVec(BitVec *bvptr, int val, int iter){
	for(int i=iter; i<bvptr->sz; i++)
		if(getBit_BitVec(bvptr, i)==val)
			return 1;
	return 0;
}

// Returns idx and len of next contiguous block of vals
void getNext_BitVec(BitVec *bvptr, int val, int *iter, int *idx, int *len){
	while(getBit_BitVec(bvptr,*iter)!=val)
		(*iter)++;
	*idx = *iter;

	while(getBit_BitVec(bvptr,*iter)==val && *iter<bvptr->sz)
		(*iter)++;
	*len = *iter-*idx;
}



void gaussBlur_BitVec(BitVec *bvptr, int rad, float th){
	float val;
	int sz = bvptr->sz;
	BitVec *tbvptr = new_BitVec(sz);

	for(int i=0; i<bvptr->sz; i++){
		val = 0;
		for(int j=i-rad; j<=i+rad; j++)
			if(j>-1 || j<sz)
				val += getBit_BitVec(bvptr,j)*GaussCoef_BitVec[rad][j-i+rad];
		if(val>th)
			setBit_BitVec(tbvptr, i, 1);
	}

	memcpy(bvptr->arr, tbvptr->arr, (bvptr->sz+7)/8);

	free_BitVec(tbvptr);
}
