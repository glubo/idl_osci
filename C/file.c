#include "dprintf.h"
#include "file.h"
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION < 12
	#define g_ascii_strtoll strtoll
#endif

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
#define VR1A "R1A="
#define VR2A "R2A="
#define VR3A "R3A="
#define VR1B "R1B="
#define VR2B "R2B="
#define VR3B "R3B="
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
		g_strstrip(lines[i]);
		if(strncmp(SVERTICAL, lines[i], strlen(SVERTICAL)) == 0){
			//Vertical info
			i++;
			g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VRANGEA, lines[i], strlen(VRANGEA)) == 0){
					ret->range_a = g_ascii_strtoll(&lines[i][strlen(VRANGEA)], NULL, 10);
					dprintf("Read_File: range_a = %d\n", ret->range_a);
				}else if(strncmp(VRANGEB, lines[i], strlen(VRANGEB)) == 0){
					ret->range_b = g_ascii_strtoll(&lines[i][strlen(VRANGEB)], NULL, 10);
					dprintf("Read_File: range_b = %d\n", ret->range_b);
				};

				i++;
				g_strstrip(lines[i]);
			}
		}else if(strncmp(STIMEBASE, lines[i], strlen(STIMEBASE)) == 0){
			//Timebase info
			i++;
			g_strstrip(lines[i]);

			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VTIMEBASE, lines[i], strlen(VTIMEBASE)) == 0){
					ret->timebase = g_ascii_strtoll(&lines[i][strlen(VTIMEBASE)], NULL, 10);
					dprintf("Read_File: timebase = %d\n", ret->timebase);
				};
				i++;
				g_strstrip(lines[i]);
			}
		}else if(strncmp(SCHANA, lines[i], strlen(SCHANA)) == 0){
			//Channel A data
			i++;
			g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VLENGHT, lines[i], strlen(VLENGHT)) == 0){
					ret->length_a = g_ascii_strtoll(&lines[i][strlen(VLENGHT)], NULL, 10);
					dprintf("Read_File: length_a = %d\n", ret->length_a);
				}else if(strncmp(DBEGIN, lines[i], strlen(DBEGIN)) == 0){
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
				};
				i++;
				if(lines[i])g_strstrip(lines[i]);
			}
		}else if(strncmp(SCHANB, lines[i], strlen(SCHANB)) == 0){
			//Channel B data
			i++;
			g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VLENGHT, lines[i], strlen(VLENGHT)) == 0){
					ret->length_b = g_ascii_strtoll(&lines[i][strlen(VLENGHT)], NULL, 10);
					dprintf("Read_File: length_b = %d\n", ret->length_b);
				}else if(strncmp(DBEGIN, lines[i], strlen(DBEGIN)) == 0){
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
				};
				i++;
				if(lines[i])g_strstrip(lines[i]);
			}
		}else if(strncmp(SSTARTSTOP, lines[i], strlen(SSTARTSTOP)) == 0){
			// Start Stop info 
			ret->had_start_stop = 1;
			i++;
			g_strstrip(lines[i]);
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
				};

				i++;
				g_strstrip(lines[i]);
			};
		}else if(strncmp(SSWEEP, lines[i], strlen(SSWEEP)) == 0){
			// Sweep info 
			// do we need this? I don't know
			i++;
			g_strstrip(lines[i]);
			while((lines[i] != NULL) && (lines[i][0] != '[')){
				if(strncmp(VBSWEEP, lines[i], strlen(VBSWEEP)) == 0){
					ret->b_sweep = g_ascii_strtoll(&lines[i][strlen(VBSWEEP)], NULL, 10);
					dprintf("Read_File: b_sweep = %d\n", ret->b_sweep);
				}else if(strncmp(VSWEEPSTEP, lines[i], strlen(VSWEEPSTEP)) == 0){
					ret->sweep_step = g_ascii_strtoll(&lines[i][strlen(VSWEEPSTEP)], NULL, 10);
					dprintf("Read_File: sweep_step = %d\n", ret->sweep_step);
				};

				i++;
				g_strstrip(lines[i]);
			};
		}else if(strncmp(SR, lines[i], strlen(SR)) == 0){
			// Resistors info 
			ret->had_R = 1;
			i++;
			g_strstrip(lines[i]);
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
				}else if(strncmp(VR1B, lines[i], strlen(VR1B)) == 0){
					ret->R1_B = g_ascii_strtoll(&lines[i][strlen(VR1B)], NULL, 10);
					dprintf("Read_File: R1_B = %lf\n", ret->R1_B);
				}else if(strncmp(VR2B, lines[i], strlen(VR2B)) == 0){
					ret->R2_B = g_ascii_strtoll(&lines[i][strlen(VR2B)], NULL, 10);
					dprintf("Read_File: R2_B = %lf\n", ret->R2_B);
				}else if(strncmp(VR3B, lines[i], strlen(VR3B)) == 0){
					ret->R3_B = g_ascii_strtoll(&lines[i][strlen(VR3B)], NULL, 10);
					dprintf("Read_File: R3_B = %lf\n", ret->R3_B);
				};

				i++;
				g_strstrip(lines[i]);
			};
		}else if(strncmp(STANDPEAK, lines[i], strlen(STANDPEAK)) == 0){
			// Time and Peaks info 
			ret->had_t_and_peak = 1;
			i++;
			g_strstrip(lines[i]);
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
				g_strstrip(lines[i]);
			};
		} else {
			//we throw away unknown line
			i++;
			g_strstrip(lines[i]);
		}
	}
	g_strfreev(lines);

	
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
	};

	if(file->had_R || force){
		if(fprintf(fp, "%s\r\n", SR)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR1A, file->R1_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR2A, file->R2_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR3A, file->R3_A)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR1B, file->R1_B)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR2B, file->R2_B)<0) error=-2;
		if(fprintf(fp, "%s%lf\r\n",VR3B, file->R3_B)<0) error=-2;
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
