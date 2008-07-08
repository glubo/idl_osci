#include "conversion.h"
#include <stdio.h>


double ItoV(int range, int I){
	switch(range){
		case 0: return (double)I*1.0e-2/32.;
		case 1: return (double)I*2.0e-2/32.;
		case 2: return (double)I*5.0e-2/32.;
		case 3: return (double)I*1.0e-1/32.;
		case 4: return (double)I*2.0e-1/32.;
		case 5: return (double)I*5.0e-1/32.;
		case 6: return (double)I*1.0/32.;
		case 7: return (double)I*2.0/32.;
		case 8: return (double)I*5.0/32.;
		default: fprintf(stderr, "I can't remember I have seen this range (%d) before, please update ItoV\n",range);return (double)I;
	}
};
double Itos(int timebase, int I){
	switch(timebase){
		case 5: return (double)I*2.0e-5;
		default: fprintf(stderr, "I can't remember I have seen this timebase (%d) before, please update Itos\n",timebase);return (double)I;
	}
};
