#include "dprintf.h"
#include "file.h"
#include "conversion.h"
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#if GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 12
	#define g_ascii_strtoll strtoll
#endif

int Analyze_File(TFile *f);

void Destroy_File(TFile *file){
	dprintf("Destroy_File\n");
	if(!file) return;
	if(file->path)free(file->path);
	file->path = 0;
	if(file->has_a){
		free(file->channel_a);
		file->channel_a = 0;
	};
	if(file->has_b){
		free(file->channel_b);
		file->channel_b = 0;
	};
	free(file);
}

#define SVERTICAL "[Vertical]"
#define STIMEBASE "[TimeBase]"
#define SCHANA "[Channel_A]"
#define SCHANB "[Channel_B]"
#define SSWEEP "[Sweep]"
#define SR "[R]"
#define SSTARTSTOP "[StartStop]"
#define STANDPEAK "[T_and_Peaks]"
#define VRANGEA "Range_A="
#define VRANGEB "Range_B="
#define VTIMEBASE "TimeBase="
#define VBSWEEP "B_Sweep="
#define VSWEEPSTEP "Sweep_Step="
#define VLENGHT "Lenght="
#define VSTARTA "STARTA="
#define VSTOPA "STOPA="
#define VSTARTB "STARTB="
#define VSTOPB "STOPB="
#define VSTARTC "STARTC="
#define VSTOPC "STOPC="
#define VSTARTNOISEA "STARTNOISEA="
#define VSTARTNOISEB "STARTNOISEB="
#define VR1A "R1A="
#define VR2A "R2A="
#define VR3A "R3A="
#define VR4A "R4A="
#define VR1B "R1B="
#define VR2B "R2B="
#define VR3B "R3B="
#define VR4B "R4B="
#define VTFALL "T_Fall_ms="
#define VPEAKA "Peak_A_C="
#define VPEAKB "Peak_B_C="
#define VPEAKC "Peak_C_C="
#define DBEGIN "--- begin ---"
#define DEND "---  end  ---"
#define DELIM "\r\n"

TFile* Read_File(const char *path){
	#define EXITREADFILE {free(fc);return ret;}
	FILE *fp = 0;
	TFile *ret = 0;
	size_t flength = 0; //file length
	typedef gchar gchar; //our type of character for loading data OS_SPECIFIC
	gchar *fc = 0; //file content
	gchar **lines = 0; //actual line of content
	int i = 0;
	
	dprintf("Read_File: path = %s\n", path);

	ret = calloc(1, sizeof(TFile));
	memset(ret, 0, sizeof(TFile));

	//we load our file into fc
	fp = fopen(path, "rb");
	if(fp==NULL)
		return ret;
	fseek(fp, 0, SEEK_END);
	flength = ftell(fp)/sizeof(gchar);
	dprintf("Read_File: flength = %d\n", flength);
	fseek(fp, 0, SEEK_SET);
	fc = calloc(flength+1, sizeof(gchar));
	if(fread(fc, sizeof(gchar), flength, fp) != flength*sizeof(gchar))
		EXITREADFILE;
	fc[flength] = 0;
	fclose(fp);
	dprintf("Read_File: file read into memory\n");

	lines = g_strsplit_set(fc, DELIM, 0);
	i = 0;
	while(lines[i] != NULL){
		if(lines[i]!=0)g_strstrip(lines[i]);
		if(strncmp(SVERTICAL, lines[i], strlen(SVERTICAL)) == 0){
			//Vertical info
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VRANGEA, lines[i], strlen(VRANGEA)) == 0){
					ret->range_a = g_ascii_strtoll(&lines[i][strlen(VRANGEA)], NULL, 10);
					dprintf("Read_File: range_a = %d\n", ret->range_a);
				}else if(strncmp(VRANGEB, lines[i], strlen(VRANGEB)) == 0){
					ret->range_b = g_ascii_strtoll(&lines[i][strlen(VRANGEB)], NULL, 10);
					dprintf("Read_File: range_b = %d\n", ret->range_b);
				};

				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			}
		}else if(strncmp(STIMEBASE, lines[i], strlen(STIMEBASE)) == 0){
			//Timebase info
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);

			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VTIMEBASE, lines[i], strlen(VTIMEBASE)) == 0){
					ret->timebase = g_ascii_strtoll(&lines[i][strlen(VTIMEBASE)], NULL, 10);
					dprintf("Read_File: timebase = %d\n", ret->timebase);
				};
				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			}
		}else if(strncmp(SCHANA, lines[i], strlen(SCHANA)) == 0){
			//Channel A data
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VLENGHT, lines[i], strlen(VLENGHT)) == 0){
					ret->length_a = g_ascii_strtoll(&lines[i][strlen(VLENGHT)], NULL, 10);
					dprintf("Read_File: length_a = %d\n", ret->length_a);
				}else if(strncmp(DBEGIN, lines[i], strlen(DBEGIN)) == 0){
					/* stara verze - problem s tim, ze se obcas data rozdeli a tim se vse posere
					 *
					int p;
					if(ret->length_a != (p = strlen(lines[i+1]))){
						fprintf(stderr, "Read_File: length_a (%d) doesn't match lenght of channel_a data (%d)", ret->length_a, p);
						ret->length_a = p;
					}
					ret->channel_a = calloc(ret->length_a, sizeof(gchar));
					memcpy(ret->channel_a, lines[i+1], ret->length_a);
					if(strncmp(DEND, lines[i+2], strlen(DEND)) != 0){
						fprintf(stderr, "Read_File: Channel_A data didn't end with ---  end  ---\n");
					}
					ret->has_a = 1;
					i += 2;
					*/

					gchar *tmp_pos;
					gchar *tmp_end;
					unsigned int tmp_len;
					tmp_pos = strstr(fc, SCHANA);
					tmp_pos = strstr(tmp_pos, DBEGIN);
					tmp_end = strstr(tmp_pos, DEND);
					tmp_end -= 1;
					tmp_pos += strlen(DBEGIN)+1;
					tmp_len = tmp_end-tmp_pos;
					if(ret->length_a != tmp_len){
						fprintf(stderr, "Read_File: length_a (%d) doesn't match lenght of channel_a data (%d)", ret->length_a, tmp_len);
						ret->length_a = tmp_len;
					}
					ret->channel_a = calloc(ret->length_a, sizeof(gchar));
					memcpy(ret->channel_a, tmp_pos, ret->length_a);
					ret->has_a = 1;
				};
				i++;
				if(lines[i])g_strstrip(lines[i]);
			}
		}else if(strncmp(SCHANB, lines[i], strlen(SCHANB)) == 0){
			//Channel B data
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VLENGHT, lines[i], strlen(VLENGHT)) == 0){
					ret->length_b = g_ascii_strtoll(&lines[i][strlen(VLENGHT)], NULL, 10);
					dprintf("Read_File: length_b = %d\n", ret->length_b);
				}else if(strncmp(DBEGIN, lines[i], strlen(DBEGIN)) == 0){
					/* stara verze - problem s tim, ze se obcas data rozdeli a tim se vse posere
					 *
					int p;
					if(ret->length_b != (p = strlen(lines[i+1]))){
						fprintf(stderr, "Read_File: length_b (%d) doesn't match lenght of channel_b data (%d)", ret->length_b, p);
						ret->length_b = p;
					}
					ret->channel_b = calloc(ret->length_b, sizeof(gchar));
					memcpy(ret->channel_b, lines[i+1], ret->length_b);
					if(strncmp(DEND, lines[i+2], strlen(DEND)) != 0){
						fprintf(stderr, "Read_File: Channel_B data didn't end with ---  end  ---\n");
					}
					i += 2;
					ret->has_b = 1;
					*/
					gchar *tmp_pos;
					gchar *tmp_end;
					unsigned int tmp_len;
					tmp_pos = strstr(fc, SCHANB);
					tmp_pos = strstr(tmp_pos, DBEGIN);
					tmp_end = strstr(tmp_pos, DEND);
					tmp_end -= 1;
					tmp_pos += strlen(DBEGIN)+1;
					tmp_len = tmp_end-tmp_pos;
					if(ret->length_b != tmp_len){
						fprintf(stderr, "Read_File: length_b (%d) doesn't match lenght of channel_b data (%d)", ret->length_b, tmp_len);
						ret->length_b = tmp_len;
					}
					ret->channel_b = calloc(ret->length_b, sizeof(gchar));
					memcpy(ret->channel_b, tmp_pos, ret->length_b);
					ret->has_b = 1;
				};
				i++;
				if(lines[i])g_strstrip(lines[i]);
			}
		}else if(strncmp(SSTARTSTOP, lines[i], strlen(SSTARTSTOP)) == 0){
			// Start Stop info 
			ret->had_start_stop = 1;
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VSTARTA, lines[i], strlen(VSTARTA)) == 0){
					ret->start_A = g_ascii_strtoll(&lines[i][strlen(VSTARTA)], NULL, 10);
					dprintf("Read_File: start_A = %d\n", ret->start_A);
				}else if(strncmp(VSTARTB, lines[i], strlen(VSTARTB)) == 0){
					ret->start_B = g_ascii_strtoll(&lines[i][strlen(VSTARTB)], NULL, 10);
					dprintf("Read_File: start_B = %d\n", ret->start_B);
				}else if(strncmp(VSTARTC, lines[i], strlen(VSTARTC)) == 0){
					ret->start_C = g_ascii_strtoll(&lines[i][strlen(VSTARTC)], NULL, 10);
					dprintf("Read_File: start_C = %d\n", ret->start_C);
				}else if(strncmp(VSTOPA, lines[i], strlen(VSTOPA)) == 0){
					ret->stop_A = g_ascii_strtoll(&lines[i][strlen(VSTOPA)], NULL, 10);
					dprintf("Read_File: stop_A = %d\n", ret->stop_A);
				}else if(strncmp(VSTOPB, lines[i], strlen(VSTOPB)) == 0){
					ret->stop_B = g_ascii_strtoll(&lines[i][strlen(VSTOPB)], NULL, 10);
					dprintf("Read_File: stop_B = %d\n", ret->stop_B);
				}else if(strncmp(VSTOPC, lines[i], strlen(VSTOPC)) == 0){
					ret->stop_C = g_ascii_strtoll(&lines[i][strlen(VSTOPC)], NULL, 10);
					dprintf("Read_File: stop_C = %d\n", ret->stop_C);
				}else if(strncmp(VSTARTNOISEA, lines[i], strlen(VSTARTNOISEA)) == 0){
					ret->start_noise_a = g_ascii_strtoll(&lines[i][strlen(VSTARTNOISEA)], NULL, 10);
					dprintf("Read_File: start_noise_a = %d\n", ret->start_noise_a);
				}else if(strncmp(VSTARTNOISEB, lines[i], strlen(VSTARTNOISEB)) == 0){
					ret->start_noise_b = g_ascii_strtoll(&lines[i][strlen(VSTARTNOISEB)], NULL, 10);
					dprintf("Read_File: start_noise_b = %d\n", ret->start_noise_b);
				};

				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			};
		}else if(strncmp(SSWEEP, lines[i], strlen(SSWEEP)) == 0){
			// Sweep info 
			// do we need this? I don't know
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VBSWEEP, lines[i], strlen(VBSWEEP)) == 0){
					ret->b_sweep = g_ascii_strtoll(&lines[i][strlen(VBSWEEP)], NULL, 10);
					dprintf("Read_File: b_sweep = %d\n", ret->b_sweep);
				}else if(strncmp(VSWEEPSTEP, lines[i], strlen(VSWEEPSTEP)) == 0){
					ret->sweep_step = g_ascii_strtoll(&lines[i][strlen(VSWEEPSTEP)], NULL, 10);
					dprintf("Read_File: sweep_step = %d\n", ret->sweep_step);
				};

				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			};
		}else if(strncmp(SR, lines[i], strlen(SR)) == 0){
			// Resistors info 
			ret->had_R = 1;
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VR1A, lines[i], strlen(VR1A)) == 0){
					ret->R1_A = g_ascii_strtoll(&lines[i][strlen(VR1A)], NULL, 10);
					dprintf("Read_File: R1_A = %lf\n", ret->R1_A);
				}else if(strncmp(VR2A, lines[i], strlen(VR2A)) == 0){
					ret->R2_A = g_ascii_strtoll(&lines[i][strlen(VR2A)], NULL, 10);
					dprintf("Read_File: R2_A = %lf\n", ret->R2_A);
				}else if(strncmp(VR3A, lines[i], strlen(VR3A)) == 0){
					ret->R3_A = g_ascii_strtoll(&lines[i][strlen(VR3A)], NULL, 10);
					dprintf("Read_File: R3_A = %lf\n", ret->R3_A);
				}else if(strncmp(VR4A, lines[i], strlen(VR4A)) == 0){
					ret->R4_A = g_ascii_strtoll(&lines[i][strlen(VR4A)], NULL, 10);
					dprintf("Read_File: R4_A = %lf\n", ret->R4_A);
				}else if(strncmp(VR1B, lines[i], strlen(VR1B)) == 0){
					ret->R1_B = g_ascii_strtoll(&lines[i][strlen(VR1B)], NULL, 10);
					dprintf("Read_File: R1_B = %lf\n", ret->R1_B);
				}else if(strncmp(VR2B, lines[i], strlen(VR2B)) == 0){
					ret->R2_B = g_ascii_strtoll(&lines[i][strlen(VR2B)], NULL, 10);
					dprintf("Read_File: R2_B = %lf\n", ret->R2_B);
				}else if(strncmp(VR3B, lines[i], strlen(VR3B)) == 0){
					ret->R3_B = g_ascii_strtoll(&lines[i][strlen(VR3B)], NULL, 10);
					dprintf("Read_File: R3_B = %lf\n", ret->R3_B);
				}else if(strncmp(VR4B, lines[i], strlen(VR4B)) == 0){
					ret->R4_B = g_ascii_strtoll(&lines[i][strlen(VR4B)], NULL, 10);
					dprintf("Read_File: R4_B = %lf\n", ret->R4_B);
				};

				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			};
		}else if(strncmp(STANDPEAK, lines[i], strlen(STANDPEAK)) == 0){
			// Time and Peaks info 
			ret->had_t_and_peak = 1;
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VTFALL, lines[i], strlen(VTFALL)) == 0){
					ret->t_fall_ms = g_ascii_strtoll(&lines[i][strlen(VTFALL)], NULL, 10);
					dprintf("Read_File: t_fall_ms = %lf\n", ret->t_fall_ms);
				}else if(strncmp(VPEAKA, lines[i], strlen(VPEAKA)) == 0){
					ret->peak_A_C = g_ascii_strtoll(&lines[i][strlen(VPEAKA)], NULL, 10);
					dprintf("Read_File: peak_A_C = %lf\n", ret->peak_A_C);
				}else if(strncmp(VPEAKB, lines[i], strlen(VPEAKB)) == 0){
					ret->peak_B_C = g_ascii_strtoll(&lines[i][strlen(VPEAKB)], NULL, 10);
					dprintf("Read_File: peak_B_C = %lf\n", ret->peak_B_C);
				}else if(strncmp(VPEAKC, lines[i], strlen(VPEAKC)) == 0){
					ret->peak_C_C = g_ascii_strtoll(&lines[i][strlen(VPEAKC)], NULL, 10);
					dprintf("Read_File: peak_C_C = %lf\n", ret->peak_C_C);
				};

				i++;
				if(lines[i]!=0)g_strstrip(lines[i]);
			};
		} else {
			//we throw away unknown line
			i++;
			if(lines[i]!=0)g_strstrip(lines[i]);
		}
	}
	g_strfreev(lines);
	ret->path = calloc(strlen(path)+1, sizeof(char));
	strcpy(ret->path, path);
	Analyze_File(ret);

	
	EXITREADFILE;
}

int Write_File(TFile *file, char *path, int force){ //on error returns non-zero
	// -1 -- fopen(path, "wb") failed
	// -2 -- fwrite failed
	FILE *fp;
	int error = 0;

	fp = fopen(path, "wb");

	if(fp==NULL){
		fprintf(stderr, "Write_File: cannot open %s for writing\n", path);
		return -1;
	}

	if(fprintf(fp, "%s\r\n", SVERTICAL)<0) error=-2;
	if(fprintf(fp, "%s%d\r\n",VRANGEA, file->range_a)<0) error=-2;
	if(fprintf(fp, "%s%d\r\n",VRANGEB, file->range_b)<0) error=-2;
	if(fprintf(fp, "%s\r\n", STIMEBASE)<0) error=-2;
	if(fprintf(fp, "%s%d\r\n",VTIMEBASE, file->timebase)<0) error=-2;
	if(fprintf(fp, "%s\r\n", SSWEEP)<0) error=-2;
	if(fprintf(fp, "%s%d\r\n",VBSWEEP, file->b_sweep)<0) error=-2;
	if(fprintf(fp, "%s%d\r\n",VSWEEPSTEP, file->sweep_step)<0) error=-2;
	if(file->has_a){
		if(fprintf(fp, "%s\r\n", SCHANA)<0) error=-2;
		if(fprintf(fp, "%s%u\r\n",VLENGHT, file->length_a)<0) error=-2;
		if(fprintf(fp, "\n%s\n", DBEGIN)<0) error=-2;
		if(fwrite(file->channel_a, sizeof(unsigned char), file->length_a, fp)<0) error=-2;
		if(fprintf(fp, "\n%s\n", DEND)<0) error=-2;
	};
	if(file->has_b){
		if(fprintf(fp, "%s\r\n", SCHANB)<0) error=-2;
		if(fprintf(fp, "%s%u\r\n",VLENGHT, file->length_b)<0) error=-2;
		if(fprintf(fp, "\n%s\n", DBEGIN)<0) error=-2;
		if(fwrite(file->channel_b, sizeof(unsigned char), file->length_b, fp)<0) error=-2;
		if(fprintf(fp, "\n%s\n", DEND)<0) error=-2;
	};

	if(file->had_start_stop || force){
		if(fprintf(fp, "%s\r\n", SSTARTSTOP)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTARTA, file->start_A)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTOPA, file->stop_A)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTARTB, file->start_B)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTOPB, file->stop_B)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTARTC, file->start_C)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTOPC, file->stop_C)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTARTNOISEA, file->start_noise_a)<0) error=-2;
		if(fprintf(fp, "%s%d\r\n",VSTARTNOISEB, file->start_noise_b)<0) error=-2;
	};

	if(file->had_R || force){
		if(fprintf(fp, "%s\r\n", SR)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR1A, file->R1_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR2A, file->R2_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR3A, file->R3_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR4A, file->R4_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR1B, file->R1_B)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR2B, file->R2_B)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR3B, file->R3_B)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR4B, file->R4_B)<0) error=-2;
	};
	if(file->had_t_and_peak || force){
		if(fprintf(fp, "%s\r\n", STANDPEAK)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VTFALL, file->t_fall_ms)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VTFALL, file->t_fall_ms)<0) error=-2;
	};

	fclose(fp);

	return error;
}


#define MAX_LENGHT 102400
int Is_Data_File(char *path){
	FILE *fp = 0;
	size_t flength = 0; //file length
	char vert[]=SVERTICAL;
	int i;
	int valid = 1;

	fp = fopen(path, "rb");
	if(fp==NULL)
		return 0;
	fseek(fp, 0, SEEK_END);
	flength = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(flength > MAX_LENGHT){
		valid = 0;
	}else for(i=0; i<strlen(vert); i++){
		if(fgetc(fp)!=vert[i]){
			valid = 0;
			break;
		};
	}

	fclose(fp);
	return valid;
}

unsigned long integrate(unsigned char *data, int start, int stop){
	int i;
	unsigned long  tmp;
	if(start > stop){
		tmp = start;
		start=stop;
		stop = tmp;
	};
	tmp = 0;
	for(i=start; i<stop; i++){
		tmp += data[i];
	};
	return tmp;
}

#define SAFETY_MARGIN 1024

unsigned long moment2(unsigned char *data, int start, int stop){
	int i;
	unsigned long  tmp;
	unsigned long  n;
	long double t2;
	if(start > stop){
		tmp = start;
		start=stop;
		stop = tmp;
	};
	tmp = 0;
	n = 0;
	t2 = 0;
	for(i=start; i<stop; i++){
		tmp += data[i]*data[i];
		if(tmp > (ULONG_MAX-SAFETY_MARGIN)){
			t2 += tmp;
			tmp = 0;
		};
	};
	if(t2>0){
		t2 +=tmp;
		return sqrt(t2);
	}else	return sqrt(tmp);
}

void Find_Min_Max(unsigned char *data, int lenght, int *min_pos, int *max_pos){	
	int i;
	unsigned char min, max;

	min = data[0];
	max = data[0];
	*min_pos = *max_pos = 0;
	for(i=0; i<lenght; i++){
		if(data[i] < min){
			*min_pos = i;
			min = data[i];
		}else if(data[i] > max){
			*max_pos = i;
			max = data[i];
		};
	};
}

void Guess_start_stop(int t_peak_max, int peak_pos, int noiseperiod, int max_t, int *start, int *stop){
	int length;
	*start  = peak_pos - t_peak_max*0.25; // MAGIC NUMBER
	if(*start < 0) *start=0;
	length = noiseperiod*ceil((double)t_peak_max/(double)noiseperiod);
	*stop = length + *start;
	if(*stop >= max_t) *stop = max_t-1;
}

void Guess_R(TFile *f){
	if(f == NULL) return;
	f->R1_A = 10e6;
	f->R1_B = 10e6;
	f->R2_A = 7e9;
	f->R2_B = 6.8e9;
	f->R3_A = 180e3;
	f->R3_B = 179e3;
	f->R4_A = 40.1e3;
	f->R4_B = 39e3;
}

int Analyze_File(TFile *f){
	int min_A_pos, max_A_pos;
	int min_max_A;
	int min_B_pos, max_B_pos;
	int t_peak_max;
	int noiseperiod = 1;

	if(f->has_a && !f->had_start_stop){
		Find_Min_Max(f->channel_a, f->length_a, &min_A_pos, &max_A_pos);
		if(max_A_pos < min_A_pos){
			f->negative = 1;
		}else{
			f->negative = 0;
		};
		min_max_A = abs(max_A_pos-min_A_pos);
		t_peak_max = min_max_A * 0.95; //empiricky odhadnuta maximalni delka peaku MAGIC NUMBER
		t_peak_max = noiseperiod * ceil((double)t_peak_max/(double)noiseperiod); //zaokrouhlime nahoru na periodu sumu

		f->start_noise_a = (f->negative?min_A_pos:max_A_pos) + t_peak_max;
		f->start_noise_a = f->length_a - noiseperiod * floor((double)(f->length_a - f->start_noise_a)/(double)noiseperiod); //zaokrouhlime dolu delku sumu na periody sumu
		if(f->start_noise_a > f->length_a){
			fprintf(stderr, "Analyze_File: start_noise_a > lenght_a\n");
			return -1;
		};
		
		f->zero_a = integrate(f->channel_a, f->start_noise_a, f->length_a)/(f->length_a-f->start_noise_a);
		Guess_start_stop(t_peak_max, max_A_pos, noiseperiod, f->length_a, &f->start_A, &f->stop_A);
		Guess_start_stop(t_peak_max, min_A_pos, noiseperiod, f->length_a, &f->start_B, &f->stop_B);
	};

	if(f->has_b && !f->had_start_stop){
		Find_Min_Max(f->channel_b, f->length_b, &min_B_pos, &max_B_pos);

		f->start_noise_b = (f->negative?max_B_pos:min_B_pos) + t_peak_max;
		f->start_noise_b = f->length_b - noiseperiod * floor((double)(f->length_b - f->start_noise_b)/(double)noiseperiod); //zaokrouhlime dolu delku sumu na periody sumu
		if(f->start_noise_b > f->length_b){
			fprintf(stderr, "Analyze_File: start_noise_b > lenght_b\n");
			return -1;
		};
		
		f->zero_b = integrate(f->channel_a, f->start_noise_b, f->length_b)/(f->length_b-f->start_noise_b);
		Guess_start_stop(t_peak_max, (f->negative?max_B_pos:min_B_pos), noiseperiod, f->length_b, &f->start_C, &f->stop_C);
	};
	
	if(!f->had_R){
		Guess_R(f);
	};

	if(f->has_a){
		f->peak_A_C = ItoC(f, 'a', integrate(f->channel_a, f->start_A, f->stop_A));
		f->peak_B_C = ItoC(f, 'a', integrate(f->channel_a, f->start_B, f->stop_B));
	}
	if(f->has_b){
		f->peak_C_C = ItoC(f, 'b', integrate(f->channel_b, f->start_C, f->stop_C));
	}

	return 0;
}


#define EXPORT_ROWS 2000 //MAGIC NUMBER
void ExportData(TFile *f,const char *path){
	FILE *fp;
	int i;
	int step;
	double sps,Apsa, Apsb;

	
	fp = fopen(path, "w");
	if(fp == NULL) return;

	sps = Itos(f->timebase, 1);
	Apsa = ItoA(f, 'a', 1);
	Apsb = ItoA(f, 'b', 1);

	if(f->has_a){
		step = f->length_a/EXPORT_ROWS;
		if(f->has_b){
			// A && B
			for(i = 0; i<f->length_a && i<f->length_b; i+=step){
				fprintf(fp,"%le\t%le\t%le\n",sps*i, Apsa*(f->channel_a[i]-f->zero_a), Apsb*(f->channel_b[i]-f->zero_b));
			};
		}else{
			// A && !B
			for(i = 0; i<f->length_a ; i+=step){
				fprintf(fp,"%le\t%le\n", sps*i, Apsa*(f->channel_a[i]-f->zero_a));
			};
		};
	}else{
		if(f->has_b){
			// !A && B
			step = f->length_b/EXPORT_ROWS;
			for(i = 0;  i<f->length_b; i+=step){
				fprintf(fp,"%le\t%le\n", sps*i, Apsb*(f->channel_b[i]-f->zero_b));
			};
		};
	};
	fclose(fp);
};
