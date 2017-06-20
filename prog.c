#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "BitVec.h"
#include "DaubDWT.h"
#include "DWTAnlz.h"
#include "HaarDWT.h"
#include "Intrpl.h"
#include "SigAnlz.h"


static int constrain(int n, int min, int max){
	if(n<min)
		return min;
	if(n>max)
		return max;
	return n;
}

// DEBUG
void printbv(BitVec *bv, float *sig){
	int iter=0;
	int idx, len;
	while(hasNext_BitVec(bv,1,iter)){
		getNext_BitVec(bv,1, &iter, &idx, &len);
		printf("%d\t%d :\t%d\t\t%d :\t%d\n", len, idx, (int)sig[idx], idx+len-1, (int)sig[idx+len-1]);
	}
	printf("\n");
}


int main(int argc, char *argv[]){

	char *sigDir;	// Dir containing signal files n.txt
	int numSig;		// Number of sigals in the dir
	char *peakDir;
	char *dwtDir;	// Dir conntaing DWTs
	char *ratFile;	// File containg ratios
	int numRat;		// Number of ratios to calculate
	char *anlzChoice;

	if(argc != 8){
		printf("sigDir numSig peakDir dwtDir ratFile numRat anlzChoice\n");
		return 0;
	}
	
	// Argument parsing

	sigDir = argv[1];
	numSig = atoi(argv[2]);
	peakDir = argv[3];
	dwtDir = argv[4];
	ratFile = argv[5];
	numRat = atoi(argv[6]);
	anlzChoice = argv[7];


	FILE *ratFilePtr = fopen(ratFile, "w");


	// Loop through all signal files

	for(int i_numSig=0; i_numSig<numSig; i_numSig++){
		char sigFile[128];	// Name of current signal file
		sprintf(sigFile, "%s/%d.txt", sigDir, i_numSig);
		printf("Reading from %s\n", sigFile);
		FILE *sigFilePtr = fopen(sigFile, "r");

		// Read signal
		int sigLen;
		fscanf(sigFilePtr, " %d", &sigLen);
		float *sig = malloc(sigLen*sizeof(float));
		for(int i=0; i<sigLen; i++)
			fscanf(sigFilePtr, " %f", &sig[i]);

		fclose(sigFilePtr);


		// Peak identification
		BitVec *bv = new_BitVec(sigLen);
		threshold_SigAnlz(sig, sigLen, bv, 2, 0.01);

		// Ignore dips
		ignoreDipBitVec_SigAnlz(bv, sig, sigLen, 2);

		// DEBUG
		// printbv(bv, sig);

		// Blur bit vector
		toggleMaxLen(bv,0,2);
		toggleMaxLen(bv,1,6);
		printbv(bv, sig);
		// gaussBlur_BitVec(bv,3,0.2);
		
		// DEBUG
		// printbv(bv, sig);


		// Loop through peaks
		int iter=0;
		for(int i_numPeak=0; hasNext_BitVec(bv,1,iter)==1; i_numPeak++){
			
			int peakIdx;	// Storing peak location
			int peakLen;
			getNext_BitVec(bv,1,&iter,&peakIdx,&peakLen);	// Get peak idx and len

			int sigNewLen = 16;
			float *sigNew = malloc(sigNewLen*sizeof(float));	// Resized signal
			cubic_Intrpl(sig+peakIdx, peakLen, sigNew, sigNewLen);
			// linear_Intrpl(sig+peakIdx, peakLen, sigNew, sigNewLen);


			// Write peak signal to file
			char peakFile[128];	// Name of current DWT file
			sprintf(peakFile, "%s/%d_%d.txt", peakDir, i_numSig, i_numPeak);
			// printf("Writing to %s\n", peakFile);
			FILE *peakFilePtr = fopen(peakFile, "w");

			fprintf(peakFilePtr, "%d\n", sigNewLen);
			for(int i=0; i<sigNewLen; i++)
				fprintf(peakFilePtr, "%f\n", sigNew[i]);

			fclose(peakFilePtr);


			int dwtLen = sigNewLen;
			float *dwt = malloc(dwtLen*sizeof(float));


			if( strcmp(anlzChoice, "haar")==0 )
				coef_1D_HaarDWT(sigNew, dwt, dwtLen);
			else if( strcmp(anlzChoice, "db2")==0 || strcmp(anlzChoice, "D4")==0 )
				coef_1D_Dx_DaubDWT(sigNew, dwt, dwtLen, 4);
			else if( strcmp(anlzChoice, "db3")==0 || strcmp(anlzChoice, "D6")==0 )
				coef_1D_Dx_DaubDWT(sigNew, dwt, dwtLen, 6);
			else if( strcmp(anlzChoice, "db4")==0 || strcmp(anlzChoice, "D8")==0 )
				coef_1D_Dx_DaubDWT(sigNew, dwt, dwtLen, 8);


			// Output to ratio file
			float rat[numRat];
			ratio_DWTAnlz(dwt, dwtLen, rat, numRat);
			fprintf(ratFilePtr, "%d\t%d\t%d\t%d\t", i_numSig, i_numPeak, peakIdx, peakLen);
			for(int i_numRat=0; i_numRat<numRat; i_numRat++)
				fprintf(ratFilePtr, "%f\t", rat[i_numRat]);
			fprintf(ratFilePtr, "\n");


			// // DEBUG
			// for(int i_numRat=0; i_numRat<numRat; i_numRat++)
			// 	printf("%f\t", rat[i_numRat]);
			// printf("\n");


			// Output DWT
			char dwtFile[128];	// Name of current DWT file
			sprintf(dwtFile, "%s/%d_%d.txt", dwtDir, i_numSig, i_numPeak);
			// printf("Writing to %s\n", dwtFile);
			FILE *dwtFilePtr = fopen(dwtFile, "w");

			fprintf(dwtFilePtr, "%d\n", dwtLen);
			for(int i=0; i<dwtLen; i++)
				fprintf(dwtFilePtr, "%f\n", dwt[i]);
			
			fclose(dwtFilePtr);

			free(sigNew);
			free(dwt);
		}

		free_BitVec(bv);
		free(sig);

	}

	fclose(ratFilePtr);

	return 0;
}