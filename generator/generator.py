#!/usr/bin/python

from math import *
def f(x,o,u,c):
	return 1./o/sqrt(2*pi)*exp(0.0-(x-u)*(x-u)/4./o/o)*c

for xx in range(0,30000):
	x=xx/30.
	print f(x,5,40,10)+sin(x/10.*pi)*0.1-f(x,5,220,10)
