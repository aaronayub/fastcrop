// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include <cairo.h>

#include "config.h"
#include "fc-app.h"
#include "fc-app-window.h"
#include "fc-options.h"

struct _FcApp {
  GtkApplication parent;
  FcOptions *options;
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
  { .name = "quit", .activate = quit_shortcut_cb }
};

static gint fc_app_handle_local_options (GApplication *app, GVariantDict *options) {
  FcApp *fc_app = FC_APP (app);
  if (g_variant_dict_contains (options, "dimensions")) {
    fc_app->options->dimensions = TRUE;
  }
  if (g_variant_dict_contains (options, "magick")) {
    fc_app->options->magick = TRUE;
  }

  return -1;
}

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
  fc_app_window_apply_options (window, FC_APP (app)->options);
  fc_app_window_open_paths (window, files[0], files[1]);
}

static void fc_app_class_init (FcAppClass *class) {
  GApplicationClass *app_class = G_APPLICATION_CLASS (class);
  app_class->handle_local_options = fc_app_handle_local_options;
  app_class->activate = fc_app_activate;
  app_class->open = fc_app_open;
}

static void fc_app_init (FcApp *app) {
  // Set default options
  app->options = malloc (sizeof (FcOptions));
  app->options->magick = FALSE;

  // Define program options
  const GOptionEntry options[] = {
    {"dimensions", 'd', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, NULL,
      "Print output dimensions to standard output in the format of WxH+X+Y", NULL},
#ifdef DEP_MAGICK
    {"magick", 'm', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, NULL,
      "Encode output files with MagickWand. Requires ImageMagick >=7.0.", NULL},
#endif
    {NULL}
  };
  g_application_add_main_option_entries (G_APPLICATION (app), options);
}

FcApp *fc_app_new (void) {
  return g_object_new (FC_APP_TYPE, "application-id", "com.github.aaronayub.fastcrop", "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
