#include "dir.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>

int Add_File(TDir *dir, const gchar *filepath, const gchar *filename){
	if(dir->nfiles + 1 > dir->filesalloc){
		dir->filesalloc *= 2;
		dir->filesalloc += 1;
		dir->files = realloc(dir->files, sizeof(TFile *)*dir->filesalloc); //TODO: check return value
	};
	dir->files[dir->nfiles] = Read_File(filepath);
	dir->nfiles += 1;
	return 0;
}

TDir * Read_Dir(char *path){
	TDir *rval = 0;
	GDir *dir = 0;
	const gchar *filename = 0;
	gchar *filepath = 0;

	rval = calloc(1, sizeof(TDir));
	memset(rval, 0, sizeof(TDir));

	dir = g_dir_open(path, 0, 0);
	while((filename = g_dir_read_name(dir)) !=NULL){
		filepath = g_strjoin(G_DIR_SEPARATOR_S, path, filename, NULL);
		if(Is_Data_File(filepath)){
			Add_File(rval, filepath, filename); //TODO:check return value
		};
		g_free(filepath);
	}
	//TODO: statistics
	rval->path = calloc(strlen(path)+1, sizeof(char));
	strcpy(rval->path, path);
	return rval;
}


void Destroy_Dir(TDir *dir){
	int i;

	if(dir){
		for(i=0; i<dir->nfiles; i++){
			Destroy_File(dir->files[i]);
		};
		free(dir->files);

		free(dir->path);
		free(dir);
	};
}
