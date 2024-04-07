// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>

#include "fc-app-window.h"

struct _FcAppWindow {
	GtkApplicationWindow parent;
  GtkWidget *draw_area;
  GdkPixbuf *pixbuf;
  
  // Crop region coordinates
  int top;
  int left;
  int bottom;
  int right;
  int width;
  int height;
};

G_DEFINE_TYPE (FcAppWindow, fc_app_window, GTK_TYPE_APPLICATION_WINDOW)

/** Update the draw area with the loaded image and cropping boundary-box */
static void update_screen (GtkDrawingArea *da, cairo_t *cr, int width, int height, gpointer user_data) {
  // Draw the source image onto the area
  FcAppWindow *window = user_data;
  GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple (window->pixbuf, width, height, 0);
  gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
  cairo_paint (cr);
  g_object_unref (pixbuf);

  // Draw the crop region.
  cairo_set_line_width (cr, 1.0);
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

  int display_left   = window->left / gdk_pixbuf_get_width (window->pixbuf) * gtk_widget_get_width (window->draw_area);
  int display_width  = window->width / gdk_pixbuf_get_width (window->pixbuf) * gtk_widget_get_width (window->draw_area);
  int display_top    = window->top / gdk_pixbuf_get_height (window->pixbuf) * gtk_widget_get_height (window->draw_area);
  int display_height = window->height / gdk_pixbuf_get_height (window->pixbuf) * gtk_widget_get_height (window->draw_area);

  cairo_rectangle (cr, display_left, display_top, display_width, display_height);
  cairo_stroke (cr);
}

static void fc_app_window_init (FcAppWindow *window) {
	gtk_widget_init_template (GTK_WIDGET (window));

  // Setup the drawing area, where the image and crop-boundaries are shown
  window->draw_area = gtk_drawing_area_new ();
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (window->draw_area), update_screen, window, NULL);
  gtk_window_set_child (GTK_WINDOW (window), window->draw_area);
}

/** Load the image file, and set up the window if the file is valid. */
void fc_app_window_open_file (FcAppWindow *window, GFile *file) {
  char *filepath = g_file_get_path (file);

  // Exit early if the file does not exist
  if (!g_file_query_exists (file, NULL)) {
    g_print ("Could not open file: %s. Exiting.\n", filepath);
    gtk_window_close (GTK_WINDOW (window));
  }

  // Load the file into a pixbuf and initialize the crop-region from it
  window->pixbuf = gdk_pixbuf_new_from_file (filepath, NULL);
  free (filepath);
  
  window->top = 0;
  window->left = 0;
  window->right  = gdk_pixbuf_get_width (window->pixbuf);
  window->width  = gdk_pixbuf_get_width (window->pixbuf);
  window->height = gdk_pixbuf_get_height (window->pixbuf);
  window->bottom = gdk_pixbuf_get_height (window->pixbuf);
}

static void fc_app_window_class_init (FcAppWindowClass *class) {}

FcAppWindow *fc_app_window_new (GtkApplication *app) {
	return g_object_new (FC_APP_WINDOW_TYPE, "application", app, NULL);
}
