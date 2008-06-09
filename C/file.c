#include "dprintf.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Destroy_File(TFile *file){
	free(file->path);
	file->path = 0;
	if(file->has_a){
		free(file->channel_a);
		file->channel_a = 0;
	};
	if(file->has_b){
		free(file->channel_b);
		file->channel_b = 0;
	};
}

TFile Read_File(char *path){
	FILE *fp;
	TFile ret;
	size_t flength; //file length
	typedef char gchar; //our type of character for loading data OS_SPECIFIC
	gchar *fc; //file content
	gchar *pos; //actual position in content
	
	memset(&ret, 0, sizeof(TFile));

	//we load our file into fc
	fp = fopen(path, "rb");
	if(fp==NULL)
		return ret;
	fseek(fp, 0, SEEK_END);
	flength = ftell(fp)/sizeof(gchar);
	dprintf("Read_File: flength = %z\n", flength);
	fseek(fp, 0, SEEK_SET);
	fc = calloc(flength, sizeof(gchar));
	if(fread(fc, sizeof(gchar), flength, fp) != flength*sizeof(gchar))
		return ret;
	fclose(fp);
	dprintf("Read_File: file read into memory\n");

	

}
