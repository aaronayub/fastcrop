// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include <cairo.h>

#include "fc-app.h"

struct _FcApp {
	GtkApplication parent;
};

G_DEFINE_TYPE (FcApp, fc_app, GTK_TYPE_APPLICATION)

/** Provide users with an error message if the app is opened with no files */
static void fc_app_activate (GApplication *app) {
	g_printerr ("Please provide the path of a file to be cropped with fastcrop.\n");
}

/** Update the draw area with the loaded image and cropping boundary-box */
static void update_screen (GtkDrawingArea *da, cairo_t *cr, int width, int height, gpointer user_data) {
}

static void fc_app_open (GApplication *app, GFile **files, int n_files, const char *hint) {
  // Setup the application window
  GtkWindow *window = GTK_WINDOW (gtk_application_window_new (GTK_APPLICATION (app)));
  gtk_window_set_title (window, "fastcrop");

  // Setup the drawing area, where the image and crop-boundaries are shown
  GtkWidget *da = gtk_drawing_area_new ();
  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (da), update_screen, NULL, NULL);
  gtk_window_set_child (window, da);

  gtk_window_present (window);
}

static void fc_app_class_init (FcAppClass *class) {
	GApplicationClass *app_class = G_APPLICATION_CLASS (class);
  app_class->activate = fc_app_activate;
  app_class->open = fc_app_open;
}

static void fc_app_init (FcApp *app) {
}

FcApp *fc_app_new (void) {
	return g_object_new (FC_APP_TYPE, "application-id", "com.github.aaronayub.fastcrop", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
