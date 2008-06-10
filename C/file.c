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
#define VRANGEA "Range_A="
#define VRANGEB "Range_B="
#define VTIMEBASE "TimeBase="
#define VLENGHT "Lenght="
#define DBEGIN "--- begin ---"
#define DEND "---  end  ---"
#define DELIM "\r\n"

TFile* Read_File(char *path){
	#define EXITREADFILE {free(fc);return ret;}
	FILE *fp = 0;
	TFile *ret = 0;
	size_t flength = 0; //file length
	typedef gchar gchar; //our type of character for loading data OS_SPECIFIC
	gchar *fc = 0; //file content
	gchar *pos = 0; //actual position in content
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
		} else i++;
	}
	g_strfreev(lines);

	
	EXITREADFILE;
}

#define MAX_LENGHT 102400
int Is_Data_File(char *path){
	FILE *fp = 0;
	size_t flength = 0; //file length
	char c;
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
