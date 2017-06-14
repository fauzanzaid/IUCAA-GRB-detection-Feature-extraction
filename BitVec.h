#ifndef c220eba242bf422fbae8bc3ae4e55552
#define c220eba242bf422fbae8bc3ae4e55552

#include <stdint.h>

typedef struct BitVec BitVec;

BitVec *new_BitVec(int sz);
BitVec *duplicate_BitVec(BitVec *bvptr);
void copy_BitVec(BitVec *src, BitVec *dest);
void free_BitVec(BitVec *bvptr);

int nSet_BitVec(BitVec *bvptr);
void setBit_BitVec(BitVec *bvptr, int idx, int val);
void setAll_BitVec(BitVec *bvptr, int val);
int getBit_BitVec(BitVec *bvptr, int idx);

int getSize_BitVec(BitVec *bvptr);

int hasNext_BitVec(BitVec *bvptr, int val, int iter);
void getNext_BitVec(BitVec *bvptr, int val, int *iter, int *idx, int *len);

void gaussBlur_BitVec(BitVec *bvptr, int rad, float th);
void toggleMaxLen(BitVec *bvptr, int val, int maxlen);

#endif
