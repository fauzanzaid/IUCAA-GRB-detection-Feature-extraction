#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>

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

	char *sigDir;		// Dir containing signal files n.txt
	int numSig;			// Number of sigals in the dir
	char *peakDir;		// p Dir to create dwt files
	float sigTh;		// th*sigma for thresholding of signal
	int maxZero;		// Max length of holes in a signal
	int minSigLen;		// l Min length of a valid signal
	float sigPadRatio;	// Added padding L/R to sig before dwt
	char *dwtDir;		// d Dir conntaing DWTs
	int dwtLenMin;		// Length of dwt analysis Min
	int dwtLenMax;		// Length of dwt analysis Max
	char *ratFile;		// File containg ratios
	int numRat;			// r Number of ratios to calculate
	char *anlzChoice;	// t
	int genMode; 		// 0:train, 1:human



	// Default values

	peakDir = NULL;
	sigTh = 2;
	maxZero = 4;
	minSigLen = 12;
	sigPadRatio = 0.5;
	dwtDir = NULL;
	dwtLenMin = 16;
	dwtLenMax = 512;
	numRat = 7;
	anlzChoice = "haar";

	

	// Argument parsing

	int option;

	while(1){

		static struct option lOpts[] = {
			{"peakdir", required_argument, 0, 256},	// Start range at 256 to avoid ASCII space
			{"threshold", required_argument, 0, 257},
			{"maxzero", required_argument, 0, 258},
			{"minlength", required_argument, 0, 259},
			{"padratio", required_argument, 0, 260},
			{"dwtdir", required_argument, 0, 261},
			{"dwtminlen", required_argument, 0, 262},
			{"dwtmaxlen", required_argument, 0, 263},
			{"trasform", required_argument, 0, 264},
			{"features", required_argument, 0, 265},
			{0,	0,	0,	0}	
		};

		option = getopt_long(argc, argv, "k:l:p:d:t:f:", lOpts, 0);

		if(option == -1)	// No more options
			break;

		switch(option){
			case 256 :
			case 'k' :
				peakDir = optarg;
				break;

			case 257 :
				sigTh = atof(optarg);
				break;

			case 258 :
				maxZero = atoi(optarg);
				break;

			case 259 :
			case 'l' :
				minSigLen = atoi(optarg);
				break;

			case 260 :
			case 'p' :
				sigPadRatio = atof(optarg);
				break;
			
			case 261 :
			case 'd' :
				dwtDir = optarg;
				break;

			case 262 :
				dwtLenMin = atoi(optarg);
				break;

			case 263 :
				dwtLenMax = atoi(optarg);
				break;

			case 264 :
			case 't' :
				anlzChoice = optarg;
				break;

			case 265 :
			case 'f' :
				numRat = atoi(optarg);
				break;

		}
	}

	if(argc-optind!=3){
		printf("Usage: a.out [options] signal_directory number_of_signals output_file\n");
		return 0;
	}

	printf("%s\n", peakDir);

	sigDir = argv[optind];
	numSig = atoi(argv[optind+1]);
	ratFile = argv[optind+2];
	genMode = 0;

	FILE *ratFilePtr = fopen(ratFile, "w");


	// Loop through all signal files

	for(int i_numSig=0; i_numSig<numSig; i_numSig++){
		char sigFile[128];	// Name of current signal file
		sprintf(sigFile, "%s/%d.txt", sigDir, i_numSig);
		printf("\33[2K\r%d of %d\t%s\t", i_numSig+1, numSig, sigFile); // \33[2K\r erases current line and does carriage return
		fflush(stdout);
		FILE *sigFilePtr = fopen(sigFile, "r");

		// Read signal
		int sigLen;
		int sigType;
		fscanf(sigFilePtr, " %d %d", &sigLen, &sigType);
		float *sig = malloc(sigLen*sizeof(float));
		for(int i=0; i<sigLen; i++)
			fscanf(sigFilePtr, " %f", &sig[i]);

		fclose(sigFilePtr);


		// Peak identification
		BitVec *bv = new_BitVec(sigLen);
		threshold_SigAnlz(sig, sigLen, bv, sigTh, 0.01);

		// Ignore dips
		ignoreDipBitVec_SigAnlz(bv, sig, sigLen, sigTh);

		// DEBUG
		// printbv(bv, sig);

		// Consolidate signal
		toggleMaxLen(bv,0,1);	// Eliminate blocks of...
		toggleMaxLen(bv,1,3);	// ...above average noise
		toggleMaxLen(bv,0,maxZero);
		toggleMaxLen(bv,1,minSigLen-1);
		
		// DEBUG
		// printbv(bv, sig);


		// Loop through peaks
		int iter=0;
		for(int i_numPeak=0; hasNext_BitVec(bv,1,iter)==1; i_numPeak++){
			
			int peakIdx;	// Storing peak location
			int peakLen;
			getNext_BitVec(bv,1,&iter,&peakIdx,&peakLen);	// Get peak idx and len


			int sigPad = roundf(sigPadRatio*peakLen);	// Convert ratio to absolute size
			peakIdx -= sigPad;	// Add padding to signal
			peakLen += 2*sigPad;
			if(peakIdx < 0)	// Bound check L
				peakIdx = 0;
			if(peakIdx+peakLen > sigLen)	// Bound check R
				peakLen = sigLen-peakIdx;

			// DEBUG
			// printf("%d %d\n", peakIdx, peakLen);

			int dwtLen = 1 << (int)ceilf(log2f(peakLen));	// Round dwtLen to the next pow of 2
			if(dwtLen < dwtLenMin)
				dwtLen = dwtLenMin;
			else if(dwtLen > dwtLenMax)
				dwtLen = dwtLenMax;


			int sigNewLen = dwtLen;
			float *sigNew = malloc(sigNewLen*sizeof(float));	// Resized signal
			cubic_Intrpl(sig+peakIdx, peakLen, sigNew, sigNewLen);
			// linear_Intrpl(sig+peakIdx, peakLen, sigNew, sigNewLen);



			// Write peak signal to file
			if(peakDir != NULL){
				char peakFile[128];	// Name of current peak file
				sprintf(peakFile, "%s/%d_%d.txt", peakDir, i_numSig, i_numPeak);
				// printf("Writing to %s\n", peakFile);
				FILE *peakFilePtr = fopen(peakFile, "w");

				fprintf(peakFilePtr, "%d\n", sigNewLen);
				for(int i=0; i<sigNewLen; i++)
					fprintf(peakFilePtr, "%f\n", sigNew[i]);

				fclose(peakFilePtr);
			}


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
			// ratioFixed1_DWTAnlz(dwt, dwtLen, rat, numRat);
			// ratioFixed2_DWTAnlz(dwt, dwtLen, rat, numRat);
			normalize_DWTAnlz(dwt, dwtLen, rat, numRat, 20);
			if(genMode==1)
				fprintf(ratFilePtr, "%d\t%d\t%d\t%d\t%d\t", i_numSig, i_numPeak, peakIdx, peakLen, sigType);
			else if(genMode==0)
				fprintf(ratFilePtr, "%d", sigType);
			for(int i_numRat=0; i_numRat<numRat; i_numRat++)
				fprintf(ratFilePtr, " %d:%f", i_numRat+1, rat[i_numRat]);
			fprintf(ratFilePtr, "\n");


			// // DEBUG
			// for(int i_numRat=0; i_numRat<numRat; i_numRat++)
			// 	printf("%f\t", rat[i_numRat]);
			// printf("\n");


			// Output DWT
			if(dwtDir!=NULL){
				char dwtFile[128];	// Name of current DWT file
				sprintf(dwtFile, "%s/%d_%d.txt", dwtDir, i_numSig, i_numPeak);
				// printf("Writing to %s\n", dwtFile);
				FILE *dwtFilePtr = fopen(dwtFile, "w");
	
				fprintf(dwtFilePtr, "%d\n", dwtLen);
				for(int i=0; i<dwtLen; i++)
					fprintf(dwtFilePtr, "%f\n", dwt[i]);
				
				fclose(dwtFilePtr);
			}


			free(sigNew);
			free(dwt);

		}

		free_BitVec(bv);
		free(sig);

	}

	fclose(ratFilePtr);

	printf("\tDone\n");

	return 0;
}