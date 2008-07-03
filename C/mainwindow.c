#define _GNU_SOURCE 1
#include <gtk/gtk.h>
#include "file.h"
#include "dir.h"
#include <unistd.h>
#include <stdlib.h>

GtkWidget *window=0;
GtkWidget *chooser=0;
GtkWidget *StartSliderA=0;
GtkWidget *StopSliderA=0;
GtkWidget *DatafileList=0;
GtkWidget *DataView=0;

TDir *g_dir=0;

enum
{
  COL_NAME = 0,
  COL_ID,
  NUM_COLS
} ;

void Plot(TFile *f){
	char *pwd;
	FILE *fp;

	pwd = get_current_dir_name();
	chdir("/tmp/plot");
	fp = fopen("plot", "w");
	if(f->has_a && f->has_b){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\nplot 'data.txt' using 0:1 with lines title 'A', 'data.txt' using 0:2 with lines title 'B'\nset output");
	}else{
		if(f->has_a){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\nplot 'data.txt' using 0:1 with lines title 'A'\nset output");
		}else if(f->has_b){
			fprintf(fp, "set terminal png\nset output 'tmp.png'\nplot 'data.txt' using 0:1 with lines title 'B'\nset output");
		};
	};
	fclose(fp);
	
	system("gnuplot plot");
	chdir(pwd);
	free(pwd);
}

void RegeneratePlot(){
	GtkTreeSelection *selection;
  GtkTreeModel     *model;
  GtkTreeIter       iter;
  guint		id;


  /* This will only work in single or browse selection mode! */

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(DatafileList));
  if (gtk_tree_selection_get_selected(selection, &model, &iter))
  {

    gtk_tree_model_get (model, &iter, COL_ID, &id, -1);

    g_print ("selected row is: %u\n", id);
	 g_mkdir_with_parents("/tmp/plot", 0777);
	 ExportData(g_dir->files[id], "/tmp/plot/data.txt");
	 Plot(g_dir->files[id]);
	 gtk_image_set_from_file(GTK_IMAGE(DataView), "/tmp/plot/tmp.png");
  }

	
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

	model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(DatafileList)));
	gtk_list_store_clear(model);
	g_dir = Read_Dir(path);
	for(i=0; i<g_dir->nfiles; i++){
		filename = SeparateFilename(g_dir->files[i]->path);
		gtk_list_store_append(model, &iter);
		gtk_list_store_set(model, &iter, COL_NAME, filename, -1);
		gtk_list_store_set(model, &iter, COL_ID, i, -1);
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

static void StartSliderA_changed( GtkRange *range, gpointer data){
	int start;
	start = gtk_range_get_value(range);
	g_print("StartSliderA: %d\n", start);
}

static void DatafileList_cursor_changed(GtkTreeView *tree_view, gpointer user_data){
	RegeneratePlot();
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
  
  store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

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
	GtkWidget *notebook;
	GtkWidget *hpaned;
	GtkWidget *frame1;
	GtkWidget *frame2;
	GtkWidget *leftvbox;

	gtk_init (&argc, &argv);
	
	hpaned = gtk_hpaned_new();
	frame1 = gtk_frame_new (NULL);
	frame2 = gtk_frame_new (NULL);
	leftvbox = gtk_vbox_new (FALSE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);


	gtk_paned_pack1 (GTK_PANED (hpaned), frame1, FALSE, FALSE);

	gtk_paned_pack2 (GTK_PANED (hpaned), frame2, TRUE, FALSE);

	StartSliderA = gtk_hscale_new_with_range(0, 35536,1);
	StopSliderA = gtk_hscale_new_with_range(0, 35536,1);
	g_signal_connect (G_OBJECT(StartSliderA), "value_changed", G_CALLBACK(StartSliderA_changed), 0);

	DatafileList = create_datafiles_view_and_model();
	g_signal_connect (G_OBJECT(DatafileList), "cursor_changed", G_CALLBACK(DatafileList_cursor_changed), 0);

	gtk_box_pack_start (GTK_BOX(leftvbox), DatafileList, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(leftvbox), StartSliderA, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX(leftvbox), StopSliderA, 0, 0, 0);

	DataView = gtk_image_new_from_file("/tmp/plot/tmp.png");
	gtk_box_pack_start (GTK_BOX(leftvbox), DataView, 1, 1, 0);


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
	notebook = gtk_notebook_new();

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hpaned, gtk_label_new("vyber"));
	/* This packs the button into the window (a gtk container). */
	gtk_container_add (GTK_CONTAINER (frame1), chooser);
	gtk_container_add (GTK_CONTAINER (frame2), leftvbox);
	gtk_container_add (GTK_CONTAINER (window), notebook);

	/* The final step is to display this newly created widget. */

	/* and the window */
	gtk_widget_show_all (window);

	/* All GTK applications must have a gtk_main(). Control ends here
	* and waits for an event to occur (like a key press or
	* mouse event). */
	gtk_main ();

	return 0;
}
