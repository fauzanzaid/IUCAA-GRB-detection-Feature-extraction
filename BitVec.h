#ifndef c220eba242bf422fbae8bc3ae4e55552
#define c220eba242bf422fbae8bc3ae4e55552

#include <stdint.h>

typedef struct BitVec BitVec;

BitVec *new_BitVec(int sz);
void free_BitVec(BitVec *bvptr);

int nSet_BitVec(BitVec *bvptr);
void setBit_BitVec(BitVec *bvptr, int idx, int val);
void setAll_BitVec(BitVec *bvptr, int val);
int getBit_BitVec(BitVec *bvptr, int idx);

int getSize_BitVec(BitVec *bvptr);

int hasNext_BitVec(BitVec *bvptr, int iter);
void getNext_BitVec(BitVec *bvptr, int *iter, int *idx, int *len);

void gaussBlur_BitVec(BitVec *bvptr, int rad, float th);

#endif
