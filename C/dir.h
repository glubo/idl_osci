#pragma once
#ifndef DIR_H_INCLUDED
#define DIR_H_INCLUDED
#include "file.h"

typedef struct{
	char *path;
	int nfiles;
	int filesalloc;
	TFile **files;

	double peak_A_C;
	double peak_A_err_C;
	double peak_B_C;
	double peak_B_err_C;
	double peak_C_C;
	double peak_C_err_C;
	double t_fall_ms;
	double t_fall_err_ms;
}TDir;

TDir * Read_Dir(char *path);
void Destroy_Dir(TDir *dir);
#endif
