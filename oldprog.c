#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "DaubDWT.h"
#include "HaarDWT.h"
#include "DWTAnlz.h"
#include "Intrpl.h"

int main(int argc, char *argv[]){

	// Declaring vars
	
	int nSig;
	char *anlzChoice;
	
	char *sigDir;
	int lenSig;
	float *sig;
	char sigFile[128];
	FILE *sigFilePtr;

	int lenSigNew;
	float *sigNew;

	char *dwtDir;
	float *dwt;
	int lenDWT;
	char dwtFile[128];
	FILE *dwtFilePtr;

	
	// Parsing args

	sigDir = argv[1];
	dwtDir = argv[2];
	nSig = atoi(argv[3]);
	anlzChoice = argv[4];
	lenSigNew = lenDWT = atoi(argv[5]);

	// Check for existenxe of dwtDir

	struct stat st;
	if (stat(dwtDir, &st) == -1) {
		mkdir(dwtDir, 0700);
	}

	// Main loop

	for(int j=0; j<nSig; j++){
		sprintf(sigFile, "%s/%d.txt", sigDir, j);
		sprintf(dwtFile, "%s/%d.txt", dwtDir, j);


		// Preparing Signal

		sigFilePtr = fopen(sigFile, "r");
		fscanf(sigFilePtr, " %d", &lenSig);
		
		sig = malloc(lenSig*sizeof(float));
		for(int i=0; i<lenSig; i++)
			fscanf(sigFilePtr, " %f", &sig[i]);

		fclose(sigFilePtr);

		sigNew = malloc(lenSigNew*sizeof(float));
		cubic_Intrpl(sig, lenSig, sigNew, lenSigNew);
		free(sig);
		
		// DWT

		dwt = malloc(lenDWT*sizeof(float));

		if( strcmp(anlzChoice, "haar")==0 )
			coef_1D_HaarDWT(sigNew, dwt, lenDWT);
		else if( strcmp(anlzChoice, "db2")==0 || strcmp(anlzChoice, "D4")==0 )
			coef_1D_Dx_DaubDWT(sigNew, dwt, lenDWT, 4);
		else if( strcmp(anlzChoice, "db3")==0 || strcmp(anlzChoice, "D6")==0 )
			coef_1D_Dx_DaubDWT(sigNew, dwt, lenDWT, 6);
		else if( strcmp(anlzChoice, "db4")==0 || strcmp(anlzChoice, "D8")==0 )
			coef_1D_Dx_DaubDWT(sigNew, dwt, lenDWT, 8);

		free(sigNew);

		// Writing op

		dwtFilePtr = fopen(dwtFile, "w");
		fprintf(dwtFilePtr, "%d\n", lenDWT);
		
		sig = malloc(lenSig*sizeof(float));
		for(int i=0; i<lenDWT; i++)
			fprintf(dwtFilePtr, "%f\n", dwt[i]);

		fclose(dwtFilePtr);

		free(dwt);
	}


	return 0;
}