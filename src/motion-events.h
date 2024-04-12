// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Event callbacks that should run when the user moves an input device, and
 * the structs that are passed as user_data. */

#pragma once

#include <gtk/gtk.h>
#include "crop-area.h"

/** Edges of the crop region that the user can move */
typedef enum {
  FC_MV_NULL = 0,
  FC_MV_TOP = 1,
  FC_MV_BOTTOM = 2,
  FC_MV_LEFT = 4,
  FC_MV_RIGHT = 8,
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

/** Returns a pointer to a new initialized MotionParams struct */
MotionParams* motion_params_new (GtkDrawingArea *da, GdkPixbuf *pb,
    CropArea *ca, MoveEdges me, int left, int top, int right, int bottom);

/** Event taking place when the user starts to drag an input device.
 * Determines which parts of the CropArea are being adjusted.
 * @param user_data A pointer to the MotionParams struct */
void select_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data);

/** Event taking place as the user continues to drag an input device.
 * Updates the values of any selected edges based on the device position.
 * @param user_data A pointer to the MotionParams struct */
void update_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data);
