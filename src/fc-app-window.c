// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include <string.h>

#include "crop-area.h"
#include "motion-events.h"
#include "fc-app-window.h"

struct _FcAppWindow {
	GtkApplicationWindow parent;
  GtkWidget *draw_area;
  GdkPixbuf *pixbuf;
  CropArea *crop_area;
  gchar *output_path;
};

G_DEFINE_TYPE (FcAppWindow, fc_app_window, GTK_TYPE_APPLICATION_WINDOW)

/** Additional parameters needed by the update_screen function */
typedef struct {
  GdkPixbuf *pixbuf;
  CropArea *crop_area;
} UpdateScreenParams;

/** Update the draw area with the loaded image and cropping boundary-box */
static void update_screen (GtkDrawingArea *draw_area, cairo_t *cr, int width, int height, gpointer user_data) {
  UpdateScreenParams *params = user_data;
  CropArea *ca = params->crop_area;
  GtkWidget *da = GTK_WIDGET (draw_area);

  // Draw the source image onto the area
  GdkPixbuf *pixbuf = gdk_pixbuf_scale_simple (params->pixbuf, width, height, 0);
  gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
  cairo_paint (cr);
  g_object_unref (pixbuf);

  // Draw the CropArea
  cairo_set_line_width (cr, 1.0);
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

  // Scale the CropArea to the size of the DrawingArea
  double rec_x = (double) ca->x / gdk_pixbuf_get_width (params->pixbuf) * gtk_widget_get_width (da);
  double rec_y = (double) ca->y / gdk_pixbuf_get_height (params->pixbuf) * gtk_widget_get_height (da);
  double rec_width = (double) ca->width / gdk_pixbuf_get_width (params->pixbuf) * gtk_widget_get_width (da);
  double rec_height = (double) ca->height / gdk_pixbuf_get_height (params->pixbuf) * gtk_widget_get_height (da);

  cairo_rectangle (cr, rec_x, rec_y, rec_width, rec_height);
  cairo_stroke (cr);
}

/** Crops the image and exits the program */
static gboolean crop_cb (GtkEventController *self, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
  if (keyval != GDK_KEY_Return &&
      keyval != GDK_KEY_KP_Enter &&
      keyval != GDK_KEY_ISO_Enter) {
    return false;
  }

  FcAppWindow *window = user_data;
  CropArea *ca = window->crop_area;
  GdkPixbuf *cropped = gdk_pixbuf_new_subpixbuf (window->pixbuf,
      ca->x, ca->y, ca->width, ca->height);

  gchar *basename = g_path_get_basename (window->output_path);
  char *extension = strrchr (basename, '.');
  if (!extension) {
    g_printerr ("Error writing to file. Please ensure the output path has an extension and try again.\n");
    exit (1);
  }
  extension++;
  if (!strcmp (extension, "jpg")) {
    extension = "jpeg";
  }

  gboolean success = gdk_pixbuf_save (
      cropped, window->output_path, extension, NULL, NULL);
  if (!success) {
    g_printerr ("Failed to write to file: %s!\n", window->output_path);
    exit (1);
  }

  g_free (basename);

  gtk_window_close (GTK_WINDOW (window));

  return true;
}

static void fc_app_window_init (FcAppWindow *window) {
  gtk_window_set_title (GTK_WINDOW (window), "fastcrop");

  // Setup the drawing area, where the image and crop-boundaries are shown
  window->draw_area = gtk_drawing_area_new ();
  gtk_window_set_child (GTK_WINDOW (window), window->draw_area);

  GtkEventController *controller = gtk_event_controller_key_new ();
  g_signal_connect (controller, "key-pressed", G_CALLBACK (crop_cb), window);
  gtk_widget_add_controller (GTK_WIDGET (window), controller);
}

/** Load the image file, and set up the window if the paths are valid. */
void fc_app_window_open_paths (FcAppWindow *window, GFile *file, GFile *output) {
  char *filepath = g_file_get_path (file);
  window->output_path = g_file_get_path (output);;

  // Exit early if the file does not exist
  if (!g_file_query_exists (file, NULL)) {
    g_print ("Could not open file: %s. Exiting.\n", filepath);
    gtk_window_close (GTK_WINDOW (window));
  }

  // Load the file into a pixbuf and initialize the crop-region from it
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (filepath, NULL);
  window->pixbuf = pixbuf;
  free (filepath);

  // Initialize parameter structs and set up callbacks using them
  CropArea *ca = malloc(sizeof(CropArea));
  ca->x = 0;
  ca->y = 0;
  ca->width = gdk_pixbuf_get_width (pixbuf);
  ca->height = gdk_pixbuf_get_height (pixbuf);

  window->crop_area = ca;

  MotionParams *mp = motion_params_new (GTK_DRAWING_AREA (window->draw_area), pixbuf, ca, FC_MV_NULL, 0, 0, gdk_pixbuf_get_width (pixbuf), gdk_pixbuf_get_height (pixbuf));

  UpdateScreenParams *usp = malloc(sizeof(UpdateScreenParams));
  usp->pixbuf = pixbuf;
  usp->crop_area = ca;

  GtkEventController *gesture_drag = GTK_EVENT_CONTROLLER (gtk_gesture_drag_new ());
  g_signal_connect (gesture_drag, "drag-begin", G_CALLBACK (select_edges), mp);
  g_signal_connect (gesture_drag, "drag-update", G_CALLBACK (update_edges), mp);
  gtk_widget_add_controller (window->draw_area, gesture_drag);

  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (window->draw_area), update_screen, usp, NULL);
}

static void fc_app_window_class_init (FcAppWindowClass *class) {}

FcAppWindow *fc_app_window_new (GtkApplication *app) {
	return g_object_new (FC_APP_WINDOW_TYPE, "application", app, NULL);
}
