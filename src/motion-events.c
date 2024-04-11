// Copyright (c) 2024 Aaron Ayub
// SPDX-License-Identifier: GPL-3.0-only

/** Event callbacks that should run when the user moves an input device, and
 * the structs that are passed as user_data. */

#include <gtk/gtk.h>
#include <math.h>

#include "motion-events.h"
#include "crop-area.h"

static inline int min (int x, int y) {
  return x < y ? x : y;
}
static inline int max (int x, int y) {
  return x > y ? x : y;
}

void select_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data) {
  MotionParams *mp = user_data;
  GtkWidget *da = GTK_WIDGET (mp->draw_area);

  double width_scale = (double) gtk_widget_get_width (da)
    / gdk_pixbuf_get_width (mp->pixbuf);
  double height_scale = (double) gtk_widget_get_height (da)
    / gdk_pixbuf_get_height (mp->pixbuf);

  int distance = 30; // Distance from which a user can select an edge.
  mp->move_edges = FC_MV_NULL;
  if (fabs(x - mp->right * width_scale) < distance) {
    mp->move_edges |= FC_MV_RIGHT;
  } else if (fabs(x - mp->left * width_scale) < distance) {
    mp->move_edges |= FC_MV_LEFT;
  }
  if (fabs(y - mp->bottom * height_scale) < distance) {
    mp->move_edges |= FC_MV_BOTTOM;
  } else if (fabs(y - mp->top * height_scale) < distance) {
    mp->move_edges |= FC_MV_TOP;
  }
}

void update_edges (GtkGestureDrag *self, gdouble x, gdouble y, gpointer user_data) {
  MotionParams *mp = user_data;
  GtkWidget *da = GTK_WIDGET (mp->draw_area);
  CropArea *ca = mp->crop_area;

  double start_x = 0;
  double start_y = 0;
  gtk_gesture_drag_get_start_point (self, &start_x, &start_y);

  int max_width = gdk_pixbuf_get_width (mp->pixbuf);
  int max_height = gdk_pixbuf_get_height (mp->pixbuf);

  int new_x = (int) ((start_x + x) / gtk_widget_get_width (da) * max_width);
  int new_y = (int) ((start_y + y) / gtk_widget_get_height (da) * max_height);

  // Recalculate edge positions
  if (mp->move_edges & FC_MV_LEFT) {
    mp->left = min (max (new_x, 0), max_width);
  } else if (mp->move_edges & FC_MV_RIGHT) {
    mp->right = min (max (new_x, 0), max_width);
  }
  if (mp->move_edges & FC_MV_TOP) {
    mp->top = min (max (new_y, 0), max_height);
  } else if (mp->move_edges & FC_MV_BOTTOM) {
    mp->bottom = min (max (new_y, 0), max_height);
  }

  // Recalculate CropArea
  ca->x = min (mp->left, mp->right);
  ca->y = min (mp->top, mp->bottom);
  ca->width = abs (mp->left - mp->right);
  ca->height = abs (mp->top - mp->bottom);

  gtk_widget_queue_draw (da);
}
