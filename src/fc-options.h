// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Options for the fastcrop program */

#pragma once

#include <gtk/gtk.h>

typedef struct {
  gchar *dimensions;
  gboolean magick;
  gboolean text;
  gchar *output_path;
} FcOptions;
