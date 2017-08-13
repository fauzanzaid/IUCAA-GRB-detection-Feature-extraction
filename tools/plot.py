#! /usr/bin/python

import sys
import matplotlib.pyplot as plt

with open(sys.argv[1], "r") as f:
	data= f.readlines()

data = data[1:]

plt.plot(data)
plt.ylabel('count')
plt.xlabel('time')	

plt.show()
