// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Event callbacks that should run when the user moves an input device, and
 * the structs that are passed as user_data. */

#pragma once

#include <gtk/gtk.h>
#include "crop-area.h"

/** Edges of the crop region that the user can move */
typedef enum {
  fc_mv_null = 0,
  fc_mv_top = 1,
  fc_mv_bottom = 2,
  fc_mv_left = 4,
  fc_mv_right = 8,
} MoveEdges;

/** Parameters for motion events that control the crop region */
typedef struct {
  GtkDrawingArea *draw_area;
  GdkPixbuf *pixbuf;
  CropArea *crop_area;
  MoveEdges move_edges;

  // Selection coordinates
  int left;
  int top;
  int right;
  int bottom;
} MotionParams;
