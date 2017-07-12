#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <limits.h>

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


void printUsageError(){
	printf(
			"Usage: a.out [options] signal_directory output_file\n"
			"options:\n"
			"--numsignals  or -m  (def: All)\n"
			"--peakdir     or -p  (def: None)\n"
			"--threshold          (def: 2)\n"
			"--maxzero            (def: 4)\n"
			"--minlength   or -l  (def: 12)\n"
			"--padratio    or -r  (def: 0.5)\n"
			"--dwtdir      or -d  (def: None)\n"
			"--dwtminlen          (def: 16)\n"
			"--dwtmaxlen          (def: 512)\n"
			"--trasform    or -t  (def: haar) : Transform choice \"haar\", \"D4\", \"D6\", \"D8\"\n"
			"--numfeatures or -n  (def: 7)    : Number of features to include in output_file\n"
		);
}


int main(int argc, char *argv[]){

	char *sigDir;		//   Dir containing signal files n.txt
	int numSig;			// m Number of sigals in the dir
	char *peakDir;		// p Dir to create peak files
	float sigTh;		// h th*sigma for thresholding of signal
	int maxZero;		//   Max length of holes in a signal
	int minSigLen;		// l Min length of a valid signal
	float sigPadRatio;	// r Added padding L/R to sig before dwt
	char *dwtDir;		// d Dir conntaing DWTs
	int dwtLenMin;		//   Length of dwt analysis Min
	int dwtLenMax;		//   Length of dwt analysis Max
	char *ratFile;		//   File containg ratios
	int numRat;			// n Number of ratios to calculate
	char *anlzChoice;	// t



	// Default values

	numSig = 0;			// Also flag
	peakDir = NULL;		// Also flag
	sigTh = 2;
	maxZero = 4;
	minSigLen = 12;
	sigPadRatio = 0.5;
	dwtDir = NULL;		// Also flag
	dwtLenMin = 16;
	dwtLenMax = 512;
	numRat = 7;
	anlzChoice = "haar";

	

	// Optoins parsing

	int option;

	while(1){

		// Start range at 256 to avoid ASCII space
		enum {
			OPT_numSig = 256,
			OPT_peakDir,
			OPT_sigTh,
			OPT_maxZero,
			OPT_minSigLen,
			OPT_sigPadRatio,
			OPT_dwtDir,
			OPT_dwtLenMin,
			OPT_dwtLenMax,
			OPT_numRat,
			OPT_anlzChoice
		};

		static struct option lOpts[] = {
			{"numsignals", 	required_argument, 0, OPT_numSig},
			{"peakdir", 	required_argument, 0, OPT_peakDir},
			{"threshold", 	required_argument, 0, OPT_sigTh},
			{"maxzero", 	required_argument, 0, OPT_maxZero},
			{"minlength", 	required_argument, 0, OPT_minSigLen},
			{"padratio", 	required_argument, 0, OPT_sigPadRatio},
			{"dwtdir", 		required_argument, 0, OPT_dwtDir},
			{"dwtminlen", 	required_argument, 0, OPT_dwtLenMin},
			{"dwtmaxlen", 	required_argument, 0, OPT_dwtLenMax},
			{"trasform", 	required_argument, 0, OPT_anlzChoice},
			{"numfeatures", required_argument, 0, OPT_numRat},
			{0,	0,	0,	0}
		};

		option = getopt_long(argc, argv, "m:p:h:l:r:d:n:t:", lOpts, 0);

		if(option == -1)	// No more options
			break;

		switch(option){
			case OPT_numSig :
			case 'm' :
				numSig = atoi(optarg);
				break;

			case OPT_peakDir :
			case 'p' :
				peakDir = optarg;
				break;

			case OPT_sigTh :
			case 'h' :
				sigTh = atof(optarg);
				break;

			case OPT_maxZero :
				maxZero = atoi(optarg);
				break;

			case OPT_minSigLen :
			case 'l' :
				minSigLen = atoi(optarg);
				break;

			case OPT_sigPadRatio :
			case 'r' :
				sigPadRatio = atof(optarg);
				break;

			case OPT_dwtDir :
			case 'd' :
				dwtDir = optarg;
				break;

			case OPT_dwtLenMin :
				dwtLenMin = atoi(optarg);
				break;

			case OPT_dwtLenMax :
				dwtLenMax = atoi(optarg);
				break;

			case OPT_anlzChoice :
			case 't' :
				anlzChoice = optarg;
				break;

			case OPT_numRat :
			case 'n' :
				numRat = atoi(optarg);
				break;
			
			default:
				printUsageError();
		}
	}



	// Arguments parsing

	if(argc-optind!=2){
		printUsageError();
		return 0;
	}

	sigDir = argv[optind];
	ratFile = argv[optind+1];

	if(numSig == 0){
		DIR *dirPtr;
		struct dirent *entry;

		dirPtr = opendir(sigDir);
		while( (entry=readdir(dirPtr)) != NULL ){
			if(entry->d_type == DT_REG)
				numSig++;
		}
	}

	DIR *dirPtr;
	dirPtr = opendir(sigDir);
	struct dirent *entPtr;


	FILE *ratFilePtr = fopen(ratFile, "w");


	// Loop through all signal files

	int i_numSig = 0;
	while( (entPtr=readdir(dirPtr))!=NULL && i_numSig<numSig){

		// Continue if entry is not a file
		if(entPtr->d_type != DT_REG)
			continue;

		char sigFile[256];	// Name of current signal file
		sprintf(sigFile, "%s/%s", sigDir, entPtr->d_name);
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
			fprintf(ratFilePtr, "%d ", sigType);
			for(int i_numRat=0; i_numRat<numRat; i_numRat++)
				fprintf(ratFilePtr, "%d:%+f ", i_numRat+1, rat[i_numRat]);
			fprintf(ratFilePtr, "%%\t%s\t%d\t%d\t%d\t%d\n", sigFile, i_numPeak, sigType, peakIdx, peakLen);


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

		i_numSig++;
	}

	closedir(dirPtr);
	fclose(ratFilePtr);

	printf("\nDone\n");

	return 0;
}