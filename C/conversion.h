#pragma once
#ifndef CONVERSION_H
#define CONVERSION_H

#include "file.h"
double ItoV(int range, double I);
double Itos(int timebase, double I);
double ItoC(TFile *f, char channel, double I);
double ItoA(TFile *f, char channel, double I);

#endif
