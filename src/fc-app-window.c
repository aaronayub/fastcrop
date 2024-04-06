// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "fc-app-window.h"

struct _FcAppWindow {
	GtkApplicationWindow parent;
  GtkWidget *draw_area;
  GdkPixbuf *source_pb;
};

G_DEFINE_TYPE (FcAppWindow, fc_app_window, GTK_TYPE_APPLICATION_WINDOW)

/** Update the draw area with the loaded image and cropping boundary-box */
static void update_screen (GtkDrawingArea *da, cairo_t *cr, int width, int height, gpointer user_data) {
}

static void fc_app_window_init (FcAppWindow *window) {
	gtk_widget_init_template (GTK_WIDGET (window));

  // Setup the drawing area, where the image and crop-boundaries are shown
  window->draw_area = gtk_drawing_area_new ();
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (window->draw_area), update_screen, NULL, NULL);
  gtk_window_set_child (GTK_WINDOW (window), window->draw_area);
}

/** Load the image file, and set up the window if the file is valid. */
void fc_app_window_open_file (FcAppWindow *window, GFile *file) {
  char *filepath = g_file_get_path (file);

  // Exit early if the file does not exist
  if (!g_file_query_exists (file, NULL)) {
    g_print ("Could not open file: %s. Exiting", filepath);
    gtk_window_close (GTK_WINDOW (window));
  }

  // Load the file into a pixbuf
  window->source_pb = gdk_pixbuf_new_from_file (filepath, NULL);
  free (filepath);
}

static void fc_app_window_class_init (FcAppWindowClass *class) {}

FcAppWindow *fc_app_window_new (GtkApplication *app) {
	return g_object_new (FC_APP_WINDOW_TYPE, "application", app, NULL);
}
