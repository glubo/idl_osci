#!/usr/bin/python

from math import *
def f(x,o,u,c):
	return 1./o/sqrt(2*pi)*exp(0.0-(x-u)*(x-u)/4./o/o)*c

for xx in range(1,30000):
	x=xx/100.
	print  f(x,5,40,10)+sin(x*2/20.*pi)*0.1-f(x,5,22,10)
