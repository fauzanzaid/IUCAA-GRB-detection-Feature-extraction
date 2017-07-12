#! /usr/bin/python3

import sys
import os
import math
from astropy.io import fits

inputFolder = sys.argv[1]
outputFolder = sys.argv[2]
eLo = int(sys.argv[3])
eHi = int(sys.argv[4])
binSize = int(sys.argv[5])

COL_TIME = 0
COL_ENERGY = 11

fnames = os.listdir(inputFolder)
printString = ""

for fidx, fname in enumerate(fnames):

	hdulist = fits.open(inputFolder+"/"+fname)

	for i in range(1,5):

		printStringLen = len(printString)
		printString = "{0} of {1} \t{2} Q{3}".format(fidx, len(fnames), fname, i)
		print(" "*printStringLen, end = "\r")
		print(printString, end="\r")

		timeStart = hdulist[i].header["TSTART"]
		timeStop = hdulist[i].header["TSTOP"]

		timeRange = timeStop - timeStart
		nBins = math.ceil(timeRange/binSize)
		count = [0]*nBins
		
		for event in hdulist[i].data:
			if(event[COL_ENERGY]>=eLo and event[COL_ENERGY]<=eHi):
				index = math.floor( nBins*(event[COL_TIME] - timeStart)/timeRange )
				count[index] += 1

		# Remove leading and trailing 0s
		while count and count[0] is 0:
			count.pop(0)
		while count and count[-1] is 0:
			count.pop(-1)
		nBins = len(count)


		sigClass = 1
		outputFile = outputFolder+"/"+os.path.splitext(fname)[0]+"_Q{0}.txt".format(i)
		with open(outputFile,'w') as f:
			f.write("{0} {1}\n".format(nBins,sigClass))
			for value in count:
				f.write("{0}\n".format(value))

	hdulist.close()

print("\nDone")
