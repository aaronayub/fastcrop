// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

#include <gtk/gtk.h>
#include "fc-app.h"

int main (int argc, char *argv[]) {
  return g_application_run (G_APPLICATION (fc_app_new ()), argc, argv);
}
