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
	int had_start_stop;
	int start_A;
	int stop_A;
	int start_B;
	int stop_B;
	int start_C;
	int stop_C;
}TFile;


TFile * Read_File(char *path);
int	Is_Data_File(char *path);
void Destroy_File(TFile *file);

#endif
