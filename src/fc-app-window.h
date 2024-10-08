// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <gtk/gtk.h>
#include "fc-options.h"

#define FC_APP_WINDOW_TYPE (fc_app_window_get_type ())
G_DECLARE_FINAL_TYPE (FcAppWindow, fc_app_window, FC, APP_WINDOW, GtkApplicationWindow)

FcAppWindow *fc_app_window_new (GtkApplication *app);

/** Load the image file, and set up the window if the path is valid. */
void fc_app_window_open_input (FcAppWindow *window, GFile *file);

/** Apply all command-line options to the app window */
void fc_app_window_apply_options (FcAppWindow *window, FcOptions *options);
