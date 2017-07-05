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



int main(int argc, char *argv[]){

	char *peakDir;	// Dir containing peak files n.txt
	int numPeak;	// Number of peaks in the dir
	char *dwtDir;	// Dir conntaing DWTs
	int dwtLen;		// Length of dwt analysis
	char *ratFile;	// File containg ratios
	int numRat;		// Number of ratios to calculate
	char *anlzChoice;
	
	if(argc != 8){
		printf(
			"Usage: ./train.out peakDir numPeak dwtDir dwtLen ratFile numRat anlzChoice\n"
			"peakDir\t\t: Directory containing peak files\n"
			"numPeak\t\t: Number of peak files in peakDir\n"
			"dwtDir\t\t: Directory to create DWT of peak files in\n"
			"dwtLen\t\t: Length of DWT files (rec 32)\n"
			"ratFile\t\t: File to output ratios(features) in IBSVM format\n"
			"numRat\t\t: Number of ratios(features) to output per peak in ratFile\n"
			"anlzChoice\t: Wavelet choice (\"haar\", \"D4\", \"D6\", \"D8\")\n"
			);
		return 0;
	}
	
	// Argument parsing

	peakDir = argv[1];
	numPeak = atoi(argv[2]);
	dwtDir = argv[3];
	dwtLen = atoi(argv[4]);
	ratFile = argv[5];
	numRat = atoi(argv[6]);
	anlzChoice = argv[7];
	

	FILE *ratFilePtr = fopen(ratFile, "w");


	// Loop through all peak files

	for(int i_numPeak=0; i_numPeak<numPeak; i_numPeak++){
		char peakFile[128];	// Name of current signal file
		sprintf(peakFile, "%s/%d.txt", peakDir, i_numPeak);
		printf("Reading from %s\t%d of %d\r", peakFile, i_numPeak+1, numPeak);
		fflush(stdout);
		FILE *peakFilePtr = fopen(peakFile, "r");

		// Read signal
		int peakLen;
		int peakClass;
		fscanf(peakFilePtr, " %d %d", &peakLen, &peakClass);
		float *peak = malloc(peakLen*sizeof(float));
		for(int i=0; i<peakLen; i++)
			fscanf(peakFilePtr, " %f", &peak[i]);

		fclose(peakFilePtr);

		int peakNewLen = dwtLen;
		float *peakNew = malloc(peakNewLen*sizeof(float));	// Resized signal
		cubic_Intrpl(peak, peakLen, peakNew, peakNewLen);


		float *dwt = malloc(dwtLen*sizeof(float));


		if( strcmp(anlzChoice, "haar")==0 )
			coef_1D_HaarDWT(peakNew, dwt, dwtLen);
		else if( strcmp(anlzChoice, "db2")==0 || strcmp(anlzChoice, "D4")==0 )
			coef_1D_Dx_DaubDWT(peakNew, dwt, dwtLen, 4);
		else if( strcmp(anlzChoice, "db3")==0 || strcmp(anlzChoice, "D6")==0 )
			coef_1D_Dx_DaubDWT(peakNew, dwt, dwtLen, 6);
		else if( strcmp(anlzChoice, "db4")==0 || strcmp(anlzChoice, "D8")==0 )
			coef_1D_Dx_DaubDWT(peakNew, dwt, dwtLen, 8);


		// Output to ratio file
		float rat[numRat];
		ratioFixed_DWTAnlz(dwt, dwtLen, rat, numRat);
		fprintf(ratFilePtr, "%d", peakClass);
		for(int i_numRat=0; i_numRat<numRat; i_numRat++)
			fprintf(ratFilePtr, " %d:%f", i_numRat, rat[i_numRat]);
		fprintf(ratFilePtr, "\n");


		// // DEBUG
		// for(int i_numRat=0; i_numRat<numRat; i_numRat++)
		// 	printf("%f\t", rat[i_numRat]);
		// printf("\n");


		// Output DWT
		char dwtFile[128];	// Name of current DWT file
		sprintf(dwtFile, "%s/%d.txt", dwtDir, i_numPeak);
		// printf("Writing to %s\n", dwtFile);
		FILE *dwtFilePtr = fopen(dwtFile, "w");

		fprintf(dwtFilePtr, "%d\n", dwtLen);
		for(int i=0; i<dwtLen; i++)
			fprintf(dwtFilePtr, "%f\n", dwt[i]);
		
		fclose(dwtFilePtr);

		free(peakNew);
		free(dwt);


		free(peak);

	}

	fclose(ratFilePtr);

	printf("\nDone\n");

	return 0;
}