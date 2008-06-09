#include <gtk/gtk.h>


static void DirChanged( GtkFileChooser *chooser, gpointer data){
	gchar *Folder;
	Folder = gtk_file_chooser_get_current_folder(chooser);
	g_print("Selected Folder: %s\n", Folder);
}

static void StartSliderA_changed( GtkRange *range, gpointer data){
	int start;
	start = gtk_range_get_value(range);
	g_print("StartSliderA: %d\n", start);
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

int main( int   argc,
          char *argv[] )
{
	/* GtkWidget is the storage type for widgets */
	GtkWidget *window;
	GtkWidget *notebook;
	GtkWidget *chooser;
	GtkWidget *hpaned;
	GtkWidget *frame1;
	GtkWidget *frame2;
	GtkWidget *leftvbox;
	GtkWidget *StartSliderA;
	GtkWidget *StopSliderA;

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
	gtk_box_pack_end (GTK_BOX(leftvbox), StartSliderA, 0, 0, 0);
	gtk_box_pack_end (GTK_BOX(leftvbox), StopSliderA, 0, 0, 0);


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
