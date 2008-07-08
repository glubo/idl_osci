#include "conversion.h"
#include <stdio.h>


double ItoV(int range, double I){
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
double Itos(int timebase, double I){
	switch(timebase){
		case 5: return (double)I*2.0e-5;
		default: fprintf(stderr, "I can't remember I have seen this timebase (%d) before, please update Itos\n",timebase);return (double)I;
	}
};

double VtoA(TFile *f, char channel, double V){
	if(channel == 'a')
		return V/f->R2_A*f->R4_A/f->R3_A;
	if(channel == 'b')
		return V/f->R2_B*f->R4_B/f->R3_B;
	return V;
}

double ItoA(TFile *f, char channel, double I){
	if(channel == 'a')
		return VtoA(f, channel, ItoV(f->range_a, I));
	if(channel == 'b')
		return VtoA(f, channel, ItoV(f->range_b, I));
	return I;
}

double ItoC(TFile *f, char channel, double I){
	return Itos(f->timebase, ItoA(f, channel, I));
}
