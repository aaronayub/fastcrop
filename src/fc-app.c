// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include <cairo.h>

#include "fc-app.h"
#include "fc-app-window.h"

struct _FcApp {
  GtkApplication parent;
};

G_DEFINE_TYPE (FcApp, fc_app, GTK_TYPE_APPLICATION)

/** Provide users with an error message if the app is opened with no files */
static void fc_app_activate (GApplication *app) {
    g_printerr ("Please run fastcrop with two arguments. A path of a file to be cropped, and the path for the output of the cropped file.\n");
}

static void quit_shortcut_cb (GSimpleAction *action, GVariant *parameter, gpointer app) {
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry action_entries[] = {
  {"quit", quit_shortcut_cb, NULL, NULL, NULL, NULL}
};

static void fc_app_open (GApplication *app, GFile **files, int n_files, const char *hint) {
  if (n_files < 2) {
    g_printerr ("Please run fastcrop with two arguments. A path of a file to be cropped, and the path for the output of the cropped file.\n");
    exit (1);
  }
  FcAppWindow *window;

  // Setup all accelerators
  g_action_map_add_action_entries (G_ACTION_MAP (app), action_entries, G_N_ELEMENTS (action_entries), app);
  const char *accels[3] = {"q", "<Control>q", NULL};
  gtk_application_set_accels_for_action (GTK_APPLICATION (app), "app.quit", accels);

  // Setup the application window
  window = fc_app_window_new (GTK_APPLICATION (app));
  gtk_window_present (GTK_WINDOW (window));
  fc_app_window_open_paths (window, files[0], files[1]);
}

static void fc_app_class_init (FcAppClass *class) {
  GApplicationClass *app_class = G_APPLICATION_CLASS (class);
  app_class->activate = fc_app_activate;
  app_class->open = fc_app_open;
}

static void fc_app_init (FcApp *app) {}

FcApp *fc_app_new (void) {
  return g_object_new (FC_APP_TYPE, "application-id", "com.github.aaronayub.fastcrop", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
