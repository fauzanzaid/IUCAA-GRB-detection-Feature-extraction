#ifndef c220eba242bf422fbae8bc3ae4e55552
#define c220eba242bf422fbae8bc3ae4e55552

typedef struct BitVec{
	char *array;
	int s;
} BitVec;

BitVec new_BitVec(int sz);
void free_BitVec(BitVec bv);

void setBit_BitVec(BitVec bv, int idx, int val);
void setAll_BitVec(BitVec bv, int val);
int getBit_BitVec(BitVec bv, int idx);

#endif
