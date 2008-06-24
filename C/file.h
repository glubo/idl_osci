#pragma once
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

typedef struct{
	char *path;
	int timebase;
	int has_a;
	int has_b;
	int range_a;
	int range_b;
	unsigned int length_a;
	unsigned int length_b;
	unsigned char *channel_a;
	unsigned char *channel_b;
	int b_sweep;
	int sweep_step;
	int had_start_stop;
	int start_A;
	int stop_A;
	int start_B;
	int stop_B;
	int start_C;
	int stop_C;
	int had_R;
	double R1_A;
	double R2_A;
	double R3_A;
	double R1_B;
	double R2_B;
	double R3_B;
	int had_t_and_peak;
	double t_fall_ms;
	double peak_A_C;
	double peak_B_C;
	double peak_C_C;
}TFile;


TFile * Read_File(const char *path);
int	Is_Data_File(char *path);
void Destroy_File(TFile *file);

#endif
