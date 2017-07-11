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

fnames = os.listdir(inputFolder)

for fidx, fname in enumerate(fnames):

	hdulist = fits.open(inputFolder+"/"+fname)

	for i in range(1,5):

		print("{0} of {1}\t{2} Q{3}".format(fidx, len(fnames), fname, i), end="\r")

		timeRange = hdulist[i].header["TSTOP"] - hdulist[i].header["TSTART"]
		nBins = math.ceil(timeRange/binSize)
		count = [0]*nBins
		
		for event in hdulist[i].data:
			if(event["ENERGY"]>=eLo and event["ENERGY"]<=eHi):
				index = math.floor( nBins*(event["Time"] - hdulist[i].header["TSTART"])/timeRange )
				count[index] += 1

		# Remove leading and trailing 0s
		while count and count[0] is 0:
			count.pop(0)
		while count and count[-1] is 0:
			count.pop(-1)


		sigClass = 1
		outputFile = outputFolder+"/"+os.path.splitext(fname)[0]+"_Q{0}.txt".format(i)
		with open(outputFile,'w') as f:
			f.write("{0} {1}\n".format(nBins,sigClass))
			for value in count:
				f.write("{0}\n".format(value))
