#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BitVec.h"



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

void free_BitVec(BitVec *bvptr){
	free(bvptr->arr);
	free(bvptr);
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
	else if(val==1)
		memset(bvptr->arr, 255, arrsz*sizeof(uint8_t));
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
