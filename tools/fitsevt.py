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

for fname in fnames:
	print(fname)
	hdulist = fits.open(inputFolder+"/"+fname)
	
	for i in range(1,5):
		timeRange = hdulist[i].header["TSTOP"] - hdulist[i].header["TSTART"]
		nBins = math.ceil(timeRange/binSize)
		count = [0]*nBins
		
		for event in hdulist[i].data:
			if(event["ENERGY"]>=eLo or event["ENERGY"]<=eHi):
				index = math.floor( nBins*(event["Time"] - hdulist[i].header["TSTART"])/timeRange )
				count[index] += 1


		sigClass = 1
		with open(outputFolder+"/{0}_{1}".format(fname,i),'w') as f:
			f.write("{0} {1}\n".format(nBins,sigClass))
			for j in range(nBins):
				f.write("{0}\n".format(count[j]))
