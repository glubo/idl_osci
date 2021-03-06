#define _GNU_SOURCE 1
#include <gtk/gtk.h>
#include "file.h"
#include "dir.h"
#include "conversion.h"
#include <unistd.h>
#include <stdlib.h>

GtkWidget *window=0;
GtkWidget *chooser=0;
GtkWidget *StartSliderA=0;
GtkWidget *StopSliderA=0;
GtkWidget *StartSliderB=0;
GtkWidget *StopSliderB=0;
GtkWidget *StartSliderC=0;
GtkWidget *StopSliderC=0;
GtkWidget *DatafileList=0;
GtkWidget *DataView=0;
int selectedid = 0;
int filechanged = 0;

TDir *g_dir=0;

enum
{
  COL_NAME = 0,
  COL_ID,
  COL_A,
  COL_B,
  COL_C,
  NUM_COLS
} ;

void Plot(TFile *f){
	char *pwd;
	FILE *fp;

	ExportData(f, "/tmp/plot/data.txt");
	pwd = get_current_dir_name();
	chdir("/tmp/plot");
	fp = fopen("plot", "w");
	if(f->has_a && f->has_b){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\n");
			fprintf(fp, "set label \"start_A\" at %lf,0 rotate point lt 4 pt 1 ps 2 offset 0,0.5\n", Itos(f->timebase, f->start_A));
			fprintf(fp, "set label \"stop_A\" at %lf,0 rotate point lt 4 pt 2 ps 2 offset 0,0.5\n", Itos(f->timebase, f->stop_A));
			fprintf(fp, "set label \"start_B\" at %lf,0 rotate point lt 4 pt 1 ps 2 offset 0,0.5\n", Itos(f->timebase, f->start_B));
			fprintf(fp, "set label \"stop_B\" at %lf,0 rotate point lt 4 pt 2 ps 2 offset 0,0.5\n", Itos(f->timebase, f->stop_B));
			fprintf(fp, "set label \"start_C\" at %lf,0 rotate point lt 4 pt 1 ps 2 offset 0,0.5\n", Itos(f->timebase, f->start_C));
			fprintf(fp, "set label \"stop_C\" at %lf,0 rotate point lt 4 pt 2 ps 2 offset 0,0.5\n", Itos(f->timebase, f->stop_C));
			fprintf(fp, "plot 'data.txt' using 1:2 with lines title 'A', 'data.txt' using 1:3 with lines title 'B'\n");
			fprintf(fp, "set output\n");
	}else{
		if(f->has_a){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\n");
			fprintf(fp, "set label \"start_A\" at %lf,0 rotate point lt 4 pt 1 ps 2 offset 0,0.5\n", Itos(f->timebase, f->start_A));
			fprintf(fp, "set label \"stop_A\" at %lf,0 rotate point lt 4 pt 2 ps 2 offset 0,0.5\n", Itos(f->timebase, f->stop_A));
			fprintf(fp, "set label \"start_B\" at %lf,0 rotate point lt 4 pt 1 ps 2 offset 0,0.5\n", Itos(f->timebase, f->start_B));
			fprintf(fp, "set label \"stop_B\" at %lf,0 rotate point lt 4 pt 2 ps 2 offset 0,0.5\n", Itos(f->timebase, f->stop_B));
			fprintf(fp, "plot 'data.txt' using 1:2 with lines title 'A'\n");
			fprintf(fp, "set output\n");
		}else if(f->has_b){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\nplot 'data.txt' using 1:2 with lines title 'B'\nset output");
		};
	};
	fclose(fp);
	
	system("gnuplot plot");
	chdir(pwd);
	free(pwd);
}

void RegeneratePlot(){
	guint		id;
	id = selectedid;
	g_mkdir_with_parents("/tmp/plot", 0777);
	Plot(g_dir->files[id]);
	gtk_image_set_from_file(GTK_IMAGE(DataView), "/tmp/plot/tmp.png");


	/* This will only work in single or browse selection mode! */


}

gchar *SeparateFilename(gchar *path){
	gchar **set=0;
	gchar *ret=0;
	int i=0;
	set = g_strsplit(path, G_DIR_SEPARATOR_S, 0);
	if(set[0] == 0){
		return 0;
	};
	while(set[i+1] != 0){
		i++;
	};
	ret = g_strdup(set[i]);
	g_strfreev(set);
	return ret;
}

void Load_Dir(gchar *path){
	int i;
	gchar *filename;
	GtkListStore *model;
	GtkTreeIter iter;
	gchar *buf;

	model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(DatafileList)));
	gtk_list_store_clear(model);
	g_dir = Read_Dir(path);
	for(i=0; i<g_dir->nfiles; i++){
		filename = SeparateFilename(g_dir->files[i]->path);
		gtk_list_store_append(model, &iter);
		gtk_list_store_set(model, &iter, COL_NAME, filename, -1);
		gtk_list_store_set(model, &iter, COL_ID, i, -1);
		buf =  g_strdup_printf("%le", g_dir->files[i]->peak_A_C);
		gtk_list_store_set(model, &iter, COL_A, buf, -1);
		g_free(buf);
		buf =  g_strdup_printf("%le", g_dir->files[i]->peak_B_C);
		gtk_list_store_set(model, &iter, COL_B, buf, -1);
		g_free(buf);
		buf =  g_strdup_printf("%le", g_dir->files[i]->peak_C_C);
		gtk_list_store_set(model, &iter, COL_C, buf, -1);
		g_free(buf);
		g_free(filename);
	}
}

static void DirChanged( GtkFileChooser *chooser, gpointer data){
	gchar *Folder;
	Folder = gtk_file_chooser_get_current_folder(chooser);
	g_print("Selected Folder: %s\n", Folder);
	Load_Dir(Folder);
	g_free(Folder);
}

static gboolean CheckFileChanged(gpointer *data){
	int i;
	if(filechanged == 0)
		return 1;
	
	filechanged = 0;
	
	for(i = 0; i < g_dir->nfiles; i++){
	//	Analyze_File(g_dir->files[i]);
	}

	RegeneratePlot();
	return 1;
}

static void StartStopSlider_changed( GtkRange *range, gpointer data){
	int *target;
	int sliderid;

	sliderid = (int)data;
	switch(sliderid){
		case 1: target = &g_dir->files[selectedid]->start_A; break;
		case 2: target = &g_dir->files[selectedid]->stop_A; break;
		case 3: target = &g_dir->files[selectedid]->start_B; break;
		case 4: target = &g_dir->files[selectedid]->stop_B; break;
		case 5: target = &g_dir->files[selectedid]->start_C; break;
		case 6: target = &g_dir->files[selectedid]->stop_C; break;
		default: fprintf(stderr, "StartStopSlider: unknow sliderid: %d\n", sliderid); return;
	};
	*target = gtk_range_get_value(range);
	g_dir->files[selectedid]->had_start_stop = 1;
	filechanged = 1;
}

void ModifyStartStopSlider(int sliderid, int value, int from, int to, int step){
	GtkRange *target;
	switch(sliderid){
		case 1: target = GTK_RANGE(StartSliderA); break;
		case 2: target = GTK_RANGE(StopSliderA); break;
		case 3: target = GTK_RANGE(StartSliderB); break;
		case 4: target = GTK_RANGE(StopSliderB); break;
		case 5: target = GTK_RANGE(StartSliderC); break;
		case 6: target = GTK_RANGE(StopSliderC); break;
		default: fprintf(stderr, "ModifyStartStopSlider: unknow sliderid: %d\n", sliderid);return;
	};
	gtk_range_set_range(target, from, to);
	gtk_range_set_increments(target, step, step);
	gtk_range_set_value(target, value);
};

void RegenerateModifyWidgets(){
	ModifyStartStopSlider(1, g_dir->files[selectedid]->start_A, 0, g_dir->files[selectedid]->length_a, 1);
	ModifyStartStopSlider(3, g_dir->files[selectedid]->start_B, 0, g_dir->files[selectedid]->length_a, 1);
	ModifyStartStopSlider(5, g_dir->files[selectedid]->start_C, 0, g_dir->files[selectedid]->length_b, 1);
	ModifyStartStopSlider(2, g_dir->files[selectedid]->stop_A, 0, g_dir->files[selectedid]->length_a, 1);
	ModifyStartStopSlider(4, g_dir->files[selectedid]->stop_B, 0, g_dir->files[selectedid]->length_a, 1);
	ModifyStartStopSlider(6, g_dir->files[selectedid]->stop_C, 0, g_dir->files[selectedid]->length_b, 1);
}

static void DatafileList_cursor_changed(GtkTreeView *tree_view, gpointer user_data){
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(DatafileList));
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, COL_ID, &selectedid, -1);

		g_print ("selected row is: %u\n", selectedid);
	}

	RegeneratePlot();
	RegenerateModifyWidgets();
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ("delete event occurred\n");

    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event". */

    return FALSE;
}

/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}


static GtkTreeModel *
create_datafiles_model (void)
{
  GtkListStore  *store;
  
  store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  return GTK_TREE_MODEL (store);
}

static GtkWidget *
create_datafiles_view_and_model (void)
{
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;
  GtkWidget           *view;

  view = gtk_tree_view_new ();



  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "ID",  
                                               renderer,
                                               "text", COL_ID,
                                               NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "Name",  
                                               renderer,
                                               "text", COL_NAME,
                                               NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "A",  
                                               renderer,
                                               "text", COL_A,
                                               NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "B",  
                                               renderer,
                                               "text", COL_B,
                                               NULL);
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,      
                                               "C",  
                                               renderer,
                                               "text", COL_C,
                                               NULL);



  model = create_datafiles_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  g_object_unref (model);

  return view;
}


int main( int   argc,
          char *argv[] )
{
	/* GtkWidget is the storage type for widgets */
	GtkWidget *SelectXModifyNotebook;
	GtkWidget *MasterHPaned;
	GtkWidget *vpanedList;
	GtkWidget *frame1;
	GtkWidget *frame2;
	GtkWidget *rightvbox;
	GtkWidget *ModifyFileVBox;
	GtkWidget *FileSelectScroll;

	gtk_init (&argc, &argv);
	
	MasterHPaned = gtk_hpaned_new();
	frame1 = gtk_frame_new (NULL);
	frame2 = gtk_frame_new (NULL);
	SelectXModifyNotebook = gtk_notebook_new();
	rightvbox = gtk_vbox_new (FALSE, 0);
	ModifyFileVBox = gtk_vbox_new (FALSE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);


	gtk_paned_pack1 (GTK_PANED (MasterHPaned), frame1, FALSE, FALSE);

	gtk_paned_pack2 (GTK_PANED (MasterHPaned), frame2, TRUE, FALSE);

	StartSliderA = gtk_hscale_new_with_range(0, 35536,1);
	StopSliderA = gtk_hscale_new_with_range(0, 35536,1);
	StartSliderB = gtk_hscale_new_with_range(0, 35536,1);
	StopSliderB = gtk_hscale_new_with_range(0, 35536,1);
	StartSliderC = gtk_hscale_new_with_range(0, 35536,1);
	StopSliderC = gtk_hscale_new_with_range(0, 35536,1);
	g_signal_connect (G_OBJECT(StartSliderA), "value_changed",  G_CALLBACK(StartStopSlider_changed), (gpointer)1);
	g_signal_connect (G_OBJECT(StopSliderA), "value_changed",   G_CALLBACK(StartStopSlider_changed), (gpointer)2);
	g_signal_connect (G_OBJECT(StartSliderB), "value_changed", G_CALLBACK(StartStopSlider_changed), (gpointer)3);
	g_signal_connect (G_OBJECT(StopSliderB), "value_changed",  G_CALLBACK(StartStopSlider_changed), (gpointer)4);
	g_signal_connect (G_OBJECT(StartSliderC), "value_changed", G_CALLBACK(StartStopSlider_changed), (gpointer)5);
	g_signal_connect (G_OBJECT(StopSliderC), "value_changed",  G_CALLBACK(StartStopSlider_changed), (gpointer)6);


	DatafileList = create_datafiles_view_and_model();
	g_signal_connect (G_OBJECT(DatafileList), "cursor_changed", G_CALLBACK(DatafileList_cursor_changed), 0);
	FileSelectScroll = gtk_scrolled_window_new(0,0);
	gtk_container_add(GTK_CONTAINER(FileSelectScroll), DatafileList);

	vpanedList = gtk_vpaned_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(SelectXModifyNotebook), FileSelectScroll, gtk_label_new("vyber"));
	gtk_notebook_append_page(GTK_NOTEBOOK(SelectXModifyNotebook), ModifyFileVBox, gtk_label_new("uprava"));
	gtk_paned_pack1 (GTK_PANED (vpanedList), SelectXModifyNotebook, TRUE, FALSE);
	gtk_paned_pack2 (GTK_PANED (vpanedList), rightvbox, FALSE, FALSE);

	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StartSliderA, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StopSliderA, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StartSliderB, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StopSliderB, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StartSliderC, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(ModifyFileVBox), StopSliderC, 0, 0, 0);

	DataView = gtk_image_new_from_file("/tmp/plot/tmp.png");
	gtk_box_pack_start (GTK_BOX(rightvbox), DataView, 1, 1, 0);


	/* This is called in all GTK applications. Arguments are parsed
	* from the command line and are returned to the application. */

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

	/* When the window is given the "delete_event" signal (this is given
	* by the window manager, usually by the "close" option, or on the
	* titlebar), we ask it to call the delete_event () function
	* as defined above. The data passed to the callback
	* function is NULL and is ignored in the callback function. */
	g_signal_connect (G_OBJECT (window), "delete_event",
			G_CALLBACK (delete_event), NULL);

	/* Here we connect the "destroy" event to a signal handler.  
	* This event occurs when we call gtk_widget_destroy() on the window,
	* or if we return FALSE in the "delete_event" callback. */
	g_signal_connect (G_OBJECT (window), "destroy",
			G_CALLBACK (destroy), NULL);

	chooser = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	g_signal_connect(G_OBJECT(chooser), "current-folder-changed", G_CALLBACK(DirChanged), NULL);

	/* This packs the button into the window (a gtk container). */
	gtk_container_add (GTK_CONTAINER (frame1), chooser);
	gtk_container_add (GTK_CONTAINER (frame2), vpanedList);
	gtk_container_add (GTK_CONTAINER (window), MasterHPaned);

	/* The final step is to display this newly created widget. */

	/* and the window */
	gtk_widget_show_all (window);

	/* All GTK applications must have a gtk_main(). Control ends here
	* and waits for an event to occur (like a key press or
	* mouse event). */
	g_timeout_add(100, (GSourceFunc)CheckFileChanged, 0);
	gtk_main ();

	return 0;
}
